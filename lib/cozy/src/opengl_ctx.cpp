#include <rvi/opengl_ctx.hpp>

#define RCFLOAT(f) *reinterpret_cast<float*>(&f)

namespace rvi
{
    rvi_cid_t opengl_ctx::_client_id;
    runtime* opengl_ctx::_runtime_ptr;
    vector2 opengl_ctx::_cursor_pos;

    opengl_ctx::opengl_ctx(runtime* rptr, rvi_cid_t cid)
    {
        _runtime_ptr = rptr;
        _client_id = cid;
        shader_utils::init_default_shaders(&_shader_program);
        glLineWidth(1);
    }

    void opengl_ctx::refresh()
    {
        _vframes.clear();
        auto snapshot = _runtime_ptr->snapshot_full_relative(_client_id);
        for(auto&& frame_entry : snapshot)
        {
            if(frame_entry.deleted)
            {
                if(_vframes.count(frame_entry.name))
                {
                    _vframes.at(frame_entry.name).line_data.clear();
                    _vframes.erase(frame_entry.name);
                }
            }
            else if (frame_entry.lines.size() > 0)
            {
                auto& entry = vframe_from_snapshot_entry(std::move(frame_entry));
                setup_vframe_ogl(entry);
            }
        }
    }

    vframe& opengl_ctx::vframe_from_snapshot_entry(relative_snapshot_entry&& entry)
    {
        const std::string key = entry.name;
        vframe vf(std::move(entry.lines));

        glGenVertexArrays(1, &(vf.vao));
        glGenBuffers(1, &(vf.vbo_pos));
        glGenBuffers(1, &(vf.vbo_col));

        _vframes.emplace(key, std::move(vf));

        return _vframes.at(key);
    }

    void opengl_ctx::setup_vframe_ogl(vframe& vf)
    {
        // Bind position vao/vbo
        glBindVertexArray(vf.vao);
        glBindBuffer(GL_ARRAY_BUFFER, vf.vbo_pos);

        // Bind data to vbo
        glBufferData(
            GL_ARRAY_BUFFER, 
            vf.line_data.size() * 4 * sizeof(float),
            vf.line_data.position_buff(),
            GL_DYNAMIC_DRAW
        );

        // Setup shader inputs
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, NULL);
        glEnableVertexAttribArray(0);

        // Bind color vbo
        glBindBuffer(GL_ARRAY_BUFFER, vf.vbo_col);
        // Bind data to vbo
        glBufferData(
            GL_ARRAY_BUFFER,
            vf.line_data.size() * 2 * sizeof(uint32_t),
            vf.line_data.color_buff(),
            GL_DYNAMIC_DRAW
        );
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4, NULL);
        glEnableVertexAttribArray(1);

        glBindVertexArray(GLUINT_NULL);
    }

    void opengl_ctx::draw(float delta_time)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glUseProgram(_shader_program);
        GLint uloc = glGetUniformLocation(_shader_program, "delta_time");
        if(uloc >= 0)
        {
            glUniform1f(uloc, delta_time);
        }
        
        for(auto &vfp : _vframes)
        {
            glBindVertexArray(vfp.second.vao);
            glDrawArrays(GL_LINES, 0, vfp.second.line_data.size() * sizeof(line));
            glBindVertexArray(GLUINT_NULL);
        }
    }

    void opengl_ctx::setup_mouse_callbacks(GLFWwindow* wnd)
    {
        glfwSetMouseButtonCallback(wnd, &mouse_press_callback);
        glfwSetCursorPosCallback(wnd, &mouse_pos_callback);
    }

    void opengl_ctx::mouse_pos_callback(GLFWwindow* wnd, double px, double py)
    {
        float fpx = static_cast<float>(px);
        float fpy = static_cast<float>(py);

        int w, h;
        glfwGetWindowSize(wnd, &w, &h);

        float x = fpx / w;
        float y = 1 - (fpy / h);
        _cursor_pos = rvi::vector2(x, y);
    }

    void opengl_ctx::mouse_press_callback(GLFWwindow* /**/, int key, int act, int /**/)
    {
        if(key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS)
        {
            auto inst = _runtime_ptr->get_instance(_client_id);
            inst->user_click(_cursor_pos);
        }
    }
}