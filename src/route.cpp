#include <beauty/route.hpp>
#include <beauty/utils.hpp>

namespace beauty
{
// --------------------------------------------------------------------------
route::route(const std::string& path, route_cb&& cb) :
        _cb(std::move(cb))
{
    if (path.empty() || path[0] !='/') {
        throw std::runtime_error("Route path [" + path + "] must begin with '/'.");
    }

    for(auto&& p : split(path, '/')) {
        _segments.emplace_back(p);
    }
}

// --------------------------------------------------------------------------
bool
route::match(beauty::request& req) const noexcept
{
    // Removes attributes and target split
    auto target_split = split(std::string_view{req.target().data(), req.target().size()}, '?');
    auto request_paths = split(target_split[0], '/');
    std::string attrs = (target_split.size() > 1 ? std::string(target_split[1]): "");

    if (_segments.size() != request_paths.size()) {
        return false;
    }

    for(std::size_t i = 0; i < _segments.size(); ++i) {
        auto& segment = _segments[i];
        auto& request_segment = request_paths[i];

        if (segment[0] == ':') {
            attrs += (attrs.empty() ? "":"&")
                    + std::string(&segment[1], segment.size() - 1)
                    + "="
                    + std::string(request_segment);
        } else if (segment != request_segment) {
            return false;
        }
    }

    if (!attrs.empty()) {
        req.get_attributes() = attributes(attrs);
    }
    return true;
}


}
