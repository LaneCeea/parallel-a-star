#pragma once

template <typename Ty>
struct Vec2 {
    Ty x, y;

    constexpr Vec2() = default;
    constexpr Vec2(Ty x, Ty y) : x(x), y(y) {}

    template <typename RTy>
    constexpr Vec2(const Vec2<RTy>& Rhs) : x(static_cast<Ty>(Rhs.x)), y(static_cast<Ty>(Rhs.y)) {}

    template <typename RTy>
    constexpr Vec2& operator=(const Vec2<RTy>& Rhs) {
        if (this == &Rhs) {
            return *this;
        }

        this->x = static_cast<Ty>(Rhs.x);
        this->y = static_cast<Ty>(Rhs.y);
        return *this;
    }

    Vec2& operator+=(const Vec2& Rhs) {
        this->x += Rhs.x;
        this->y += Rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& Rhs) {
        this->x -= Rhs.x;
        this->y -= Rhs.y;
        return *this;
    }
};

template <typename Ty>
Vec2<Ty> operator+(Vec2<Ty> Lhs, const Vec2<Ty>& Rhs) {
    Lhs += Rhs;
    return Lhs;
}

template <typename Ty>
Vec2<Ty> operator-(Vec2<Ty> Lhs, const Vec2<Ty>& Rhs) {
    Lhs -= Rhs;
    return Lhs;
}

template <typename Ty>
constexpr bool operator==(const Vec2<Ty>& Lhs, const Vec2<Ty>& Rhs) {
    return (Lhs.x == Rhs.x) && (Lhs.y == Rhs.y);
}

template <typename Ty>
constexpr bool operator!=(const Vec2<Ty>& Lhs, const Vec2<Ty>& Rhs) {
    return !(Lhs == Rhs);
}
