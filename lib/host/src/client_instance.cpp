#include <rvi/client_instance.hpp>

#include <map>

#include <rvi/runtime.hpp>

using std::vector;
using std::string;
using std::unordered_map;

namespace rvi
{
    client_instance::client_instance(runtime* rptr)
        : _lua_ctx(*this)
        , _runtime_ptr(rptr)
    {
        _ctx = std::make_unique<client_context>();
    }

    client_context* client_instance::get_context()
    {
        return _ctx.get();
    }

    void client_instance::define_macro(const string& name, const vector<string>& funs)
    {
        if(_macros.count(name) > 0)
        {
            _macros.erase(name);
        }
        _macros.emplace(name, funs);
    }

    void client_instance::undefine_macro(const std::string& name)
    {
        _macros.erase(name);
    }

    void client_instance::exec_macro(const std::string& mname)
    {
        if(_macros.count(mname) > 0)
        {
            auto& macro = _macros[mname];
            for(auto& entry : macro)
            {
                _lua_ctx.exec_script(entry);
            }
        }
    }

    const vector<string>& client_instance::get_macro(const string& name)
    {
        if(_macros.count(name) > 0)
        {
            return _macros[name];
        }
        return vector<string>();
    }

    void client_instance::set_current_frame_clickable(const std::string& call)
    {
        frame* fptr = _ctx->selected_frame();
        clickable_frame_data fdata(fptr, call);
        _clickable_frames.emplace(fptr, fdata);
    }

    void client_instance::unset_current_frame_clickable()
    {
        _clickable_frames.erase(_ctx->selected_frame());
    }

    void client_instance::user_click(vector2 pos)
    {
        auto& ctx_frames = _ctx->frames();
        std::map<uint64_t, frame*> hits;
        vector<frame*> pending_delete;

        // Get hits
        for(auto& entry : _clickable_frames)
        {
            clickable_frame_data& fdata = entry.second;
            if(fdata.rect.contains(pos))
            {
                frame* fptr = fdata.fptr;
                if(ctx_frames.count(fptr) == 0)
                {
                    pending_delete.push_back(fptr);
                }
                else
                {
                    hits.emplace(fdata.uid, fptr);
                }
            }
        }

        // Remove deleted hit frames
        for(frame* fptr : pending_delete)
        {
            _clickable_frames.erase(fptr);
        }

        // Hit frame with the latest uid (added last)
        auto pair = *(hits.rbegin()); // std::map keys are ordered
        auto& binding_name = _clickable_frames[pair.second].binding_name;
        _runtime_ptr->exec_binding(*this, binding_name, pair.second);
    }

    const std::unordered_map<std::string, std::vector<std::string>>& client_instance::macros()
    {
        return _macros;
    }

    vector<line> client_instance::snapshot_full_flat() const
    {
        return _ctx->snapshot_full_flat();
    }

    relative_snapshot client_instance::snapshot_full_relative()
    {
        return _ctx->snapshot_full_relative();
    }

    relative_snapshot client_instance::snapshot_diff_relative()
    {
        return _ctx->snapshot_diff_relative();
    }

    void client_instance::cleanup_clickable_frames()
    {
        auto& children = _ctx->frames();
        vector<frame*> pending_deletion;
        for(auto& entry : _clickable_frames)
        {
            frame* fptr = entry.first;
            if(children.count(fptr) == 0)
            {
                pending_deletion.push_back(fptr);
            }
        }
        for(frame* fptr : pending_deletion)
        {
            _clickable_frames.erase(fptr);
        }
    }
}