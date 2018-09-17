#include "transform2.h"

namespace rvi
{
    Transform2::Transform2(Vector2 pos, Vector2 scale, float rot) noexcept
        : Position(pos)
        , Scale(scale)
        , Rotation(rot)
    { }

    void Transform2::ApplyTo(std::vector<Line>& vecs) const noexcept
    {
        for (auto& v : vecs)
        {
            v.ApplyTransform(*this);
        }
    }

    Transform2 Transform2::Merge(const Transform2& other) noexcept
    {
        Transform2 res = *this;
        res.Position += other.Position;
        res.Rotation += Math::ClampAngleDeg(other.Rotation);
        res.Scale *= other.Scale;
        return res;
    }
}