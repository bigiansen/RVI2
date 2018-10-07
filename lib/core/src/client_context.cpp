#include "client_context.hpp"

#include <sstream>
#include <iterator>

namespace rvi
{ 
    const char client_context::FRAMEPATH_SEPARATOR = ':';
    const std::string client_context::MAIN_FRAMENAME = "__MAINFRAME__";

    #if RVI_COMPILER_MSVC
    #pragma warning(suppress: 26439)
    #endif
    client_context::client_context()
    {
        _main_frame = std::make_unique<frame>(MAIN_FRAMENAME);
        _frame_stack.push_back(_main_frame.get());
        _selected_frame = _main_frame.get();
    }

    client_context::client_context(client_context&& mv_src)
    {
        _frame_stack = std::move(mv_src._frame_stack);
        _main_frame = std::move(mv_src._main_frame);
        _selected_frame = _main_frame.get();

        rvi_assert(_selected_frame->name() == MAIN_FRAMENAME, "Main frame seems to be corrupted...");
        
        _local_definitions = std::move(mv_src._local_definitions);
        _modified_fpaths = std::move(mv_src._modified_fpaths);
        _cached_fpath_rebuild = mv_src._cached_fpath_rebuild;
        _current_color = mv_src._current_color;
        _cached_fpath = mv_src._cached_fpath;
        // -- regenerate frame-stack after move ------
        
        std::string stack_path = get_fpath();
        auto path_segments = str_split(stack_path, FRAMEPATH_SEPARATOR);

        rvi_assert(path_segments[0] == MAIN_FRAMENAME, "Generated an invalid framepath (corrupted framestack?)");
        
        frame* current_frame = _main_frame.get();
        _frame_stack.push_back(current_frame);
        for(auto it = path_segments.begin() +1; it != path_segments.end(); it++)
        {
            auto fname = *it;
            current_frame = current_frame->get_child(fname);
            _frame_stack.push_back(current_frame);
        }
    }

    void client_context::draw_line(vector2 from, vector2 to)
    {
        line ln(vertex(from, _current_color),
                vertex(to, _current_color));

        _selected_frame->add_line(std::move(ln));
        mark_frame_modified();
    }

    void client_context::draw_line(vector2 from, color_rgba from_color, vector2 to, color_rgba to_color)
    {
        _selected_frame->add_line(line(vertex(from, from_color), vertex(to, to_color)));
        mark_frame_modified();
    }

    void client_context::draw_line(vertex from, vertex to)
    {        
        _selected_frame->add_line(line(from, to));
        mark_frame_modified();
    }

    void client_context::draw_line(line&& ln)
    {
        _selected_frame->add_line(std::move(ln));
        mark_frame_modified();
    }

    void client_context::draw_line(const line& ln)
    {
        _selected_frame->add_line(ln);
        mark_frame_modified();
    }

    void client_context::select_frame(const std::string& name)
    {
        if (!_selected_frame->contains_child(name))
        {
            _selected_frame = _selected_frame->add_child(name);
        }
        else
        {
            _selected_frame = _selected_frame->get_child(name);
        }
        _frame_stack.push_back(_selected_frame);
        _cached_fpath_rebuild = true;
    }

    void client_context::select_frame(std::string&& name)
    {
        if (!_selected_frame->contains_child(name))
        {
            _selected_frame = _selected_frame->add_child(std::move(name));
        }
        else
        {
            _selected_frame = _selected_frame->get_child(name);
        }
        _frame_stack.push_back(_selected_frame);
        _cached_fpath_rebuild = true;
    }

    bool client_context::release_frame()
    {
        if (_selected_frame == _main_frame.get())
        {
            return false;
        }
        _cached_fpath_rebuild = true;
        _frame_stack.pop_back();
        _selected_frame = _frame_stack.back();
        return true;
    }

    bool client_context::delete_frame(const std::string& name)
    {
        return _selected_frame->delete_child(name);
    }

    const frame* client_context::selected_frame() const noexcept
    {
        return _selected_frame;
    }

    bool client_context::is_root_frame_selected() const noexcept
    {
        return _main_frame.get() == _selected_frame;
    }

    void client_context::set_color(color_rgba color) noexcept
    {
        _current_color = color;
    }

    void client_context::set_transform(const transform2& tform) noexcept
    {
        _selected_frame->set_transform(tform);
        mark_frame_modified();
    }

    void client_context::set_transform(transform2&& tform) noexcept
    {
        _selected_frame->set_transform(std::move(tform));
        mark_frame_modified();
    }

    const transform2& client_context::transform() const noexcept
    {
        return _selected_frame->transform();
    }

    void client_context::set_position(vector2 offset) noexcept
    {
        _selected_frame->set_position(offset);
        mark_frame_modified();
    }

    void client_context::set_rotation(float rotation) noexcept
    {
        _selected_frame->set_rotation(rotation);
        mark_frame_modified();
    }

    void client_context::set_scale(vector2 scale) noexcept
    {
        _selected_frame->set_scale(scale);
        mark_frame_modified();
    }

    vector2 client_context::position() const noexcept
    {
        return _selected_frame->transform().position;
    }

    float client_context::rotation() const noexcept
    {
        return _selected_frame->transform().rotation;
    }

    vector2 client_context::scale() const noexcept
    {
        return _selected_frame->transform().scale;
    }

    size_t client_context::frame_count() const noexcept
    {
        size_t result = 1;
        result += _main_frame->child_count(true);
        return result;
    }

    void client_context::clear_frame() noexcept
    {
        _selected_frame->clear_lines();
        mark_frame_modified();
    }

    void client_context::add_definition(const definition& instruction)
    {
        DISCARD_RESULT _local_definitions.emplace(instruction.name(), instruction);
    }

    void client_context::add_definition(definition&& instruction)
    {
        auto name = instruction.name();
        DISCARD_RESULT _local_definitions.emplace(name, instruction);
    }    

    void client_context::delete_definition(const std::string& name)
    {
        _local_definitions.erase(name);
    }

    bool client_context::execute_definition(const std::string& def_name)
    {
        if (_local_definitions.count(def_name) == 0)
        {
            return false;
        }
        definition def = _local_definitions.at(def_name);
        def.execute_on_context(*this);
        return true;
    }

    bool client_context::contains_definition(const std::string& def_name)
    {
        return _local_definitions.count(def_name) > 0;
    }

    const std::string& client_context::get_fpath()
    {
        static const std::string separator = std::string(1, FRAMEPATH_SEPARATOR);

        if (_cached_fpath_rebuild)
        {
            _cached_fpath.clear();
            _cached_fpath = _frame_stack[0]->name();
            for (size_t i = 1; i < _frame_stack.size(); i++)
            {
                _cached_fpath.append(separator);
                _cached_fpath.append(_frame_stack[i]->name());
            }
            _cached_fpath_rebuild = false;
        }
        return _cached_fpath;
    }

    const frame* client_context::find_frame(const std::string& fpath)
    {
        std::stringstream ss(fpath);
        std::string aux;
        frame* currentFrame = nullptr;
        transform2 currentTransform;

        currentFrame = _main_frame.get();

        while (std::getline(ss, aux, FRAMEPATH_SEPARATOR))
        {
            if (aux != MAIN_FRAMENAME)
            {
                currentFrame = currentFrame->get_child(aux);
            }
        }
        return currentFrame;
    }

    const std::pair<frame*, transform2> client_context::find_frame_with_mod_tform(const std::string& fpath)
    {
        std::stringstream ss(fpath);
        std::string aux;
        frame* currentFrame = _main_frame.get();

        transform2 currentTransform = currentFrame->transform();

        while (std::getline(ss, aux, FRAMEPATH_SEPARATOR))
        {
            if (aux != MAIN_FRAMENAME)
            {
                currentFrame = currentFrame->get_child(aux);
                currentTransform.merge_in_place(currentFrame->transform());
            }
        }
        std::pair<frame*, transform2> pair(currentFrame, std::move(currentTransform));
        return pair;
    }

    void client_context::mark_frame_modified()
    {
        const std::string& fpath = get_fpath();
        if (_modified_fpaths.count(fpath) == 0)
        {
            DISCARD_RESULT _modified_fpaths.insert(fpath);
        }
    }

    std::vector<line> client_context::snapshot_full_flat() const
    {
        return _main_frame->get_all_modulated_lines(transform2::default_value());
    }

    std::vector<line> client_context::snapshot_diff_flat()
    {
        std::vector<line> result;
        for (auto& fpath : _modified_fpaths)
        {
            auto frame_tform = find_frame_with_mod_tform(fpath);
            const frame* frm = frame_tform.first;
            const transform2 tform = frame_tform.second;

            std::vector<line> lines = frm->get_manually_modulated_lines(tform);
            std::move(lines.begin(), lines.end(), std::back_inserter(result));
        }
        _modified_fpaths.clear();
        return result;
    }

    relative_snapshot_t client_context::snapshot_diff_relative()
    {
        std::unordered_map<std::string, std::vector<line>> result;
        for (const auto& fpath : _modified_fpaths)
        {
            auto frame_tform = find_frame_with_mod_tform(fpath);
            const frame* frm = frame_tform.first;
            const transform2 tform = frame_tform.second;

            auto entry = std::make_pair(fpath, frm->get_manually_modulated_lines(tform));
            result.insert(std::move(entry));
        }
        _modified_fpaths.clear();
        return result;
    }
}