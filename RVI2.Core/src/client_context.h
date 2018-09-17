#pragma once

#include <string>
#include <stack>
#include <unordered_map>

#include "rvi_base.h"
#include "frame.h"
#include "line.h"

namespace rvi
{
    class ClientContext
    {
    private:
        const std::string MAIN_FRAMENAME = "__MAINFRAME__";
        const std::string FRAME_HIERARCHY_SEPARATOR = "~";

        Frame _mainFrame;
        Frame& _selectedFrame;
        std::stack<Frame&> _frameStack;
        U64 _contextId;

    public:
        ClientContext() noexcept;

        void DrawLine(Vector2 from, Vector2 to);
        void DrawLine(Vector2 from, ColorRGBA fromColor, Vector2 to, ColorRGBA toColor);
        void DrawLine(Vertex from, Vertex to);

        void SelectFrame(const std::string& name);
        void SelectFrame(std::string&& name);

        void ReleaseFrame();

        void SetCurrentColor(ColorRGBA color);

        void SetCurrentTransform(const Transform2& tform);
        void SetCurrentTransform(Transform2&& tform);

        const Transform2& GetCurrentTransform();

        void SetCurrentOffset(Vector2 offset);
        void SetCurrentRotation(float rotation);
        void SetCurrentScale(Vector2 scale);

        Vector2 GetCurrentOffset();
        float GetCurrentRotation();
        Vector2 GetCurrentScale();

        void ClearFrame();
    private:

    };
}