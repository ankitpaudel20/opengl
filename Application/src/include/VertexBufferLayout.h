#pragma once

#include <vector>
#include <map>
#include "core.h"


struct VertexBufferElement {
    unsigned type;
    unsigned count;
    unsigned normalized;

    [[nodiscard]] unsigned GetTypeSize() const {
        switch (type) {
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_INT:
                return sizeof(unsigned);
            case GL_UNSIGNED_BYTE:
                return sizeof(unsigned char);
            case GL_INT:
                return sizeof(int);
        }
        ASSERT(false);
        return 0;
    }

};


class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_elements;
    //std::map<std::string, VertexBufferElement> m_elementmap;
    unsigned m_stride;
public:
    VertexBufferLayout() : m_stride(0) {}

    ~VertexBufferLayout() = default;

    template<class T>
    inline void push(unsigned count, unsigned normalized);

#ifdef _MSC_VER
    template<>
    inline void push<float>(unsigned count, unsigned normalized);

    template<>
    inline void push<int>(unsigned count, unsigned normalized);

    template<>
    inline void push<unsigned>(unsigned count, unsigned normalized);

    template<>
    inline void push<unsigned char>(unsigned count, unsigned normalized);
#endif

    [[nodiscard]] inline const std::vector<VertexBufferElement> &GetElements() const { return m_elements; }

    [[nodiscard]] inline unsigned GetStride() const { return m_stride; }
 
};

template<class T>
inline void VertexBufferLayout::push(unsigned count, unsigned normalized) {
    assert(false);
}

template<>
inline void VertexBufferLayout::push<float>(unsigned count, unsigned normalized) {
    m_elements.push_back({GL_FLOAT, count, normalized});
    m_stride += count * m_elements.back().GetTypeSize();
}

template<>
inline void VertexBufferLayout::push<int>(unsigned count, unsigned normalized) {
    m_elements.push_back({GL_INT, count, normalized});
    m_stride += count * m_elements.back().GetTypeSize();
}


template<>
inline void VertexBufferLayout::push<unsigned>(unsigned count, unsigned normalized) {
    m_elements.push_back({GL_UNSIGNED_INT, count, normalized});
    m_stride += count * (m_elements.empty() ? 0 : m_elements.back().GetTypeSize());
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned count, unsigned normalized) {
    m_elements.push_back({GL_UNSIGNED_BYTE, count, normalized});
    m_stride += count * (m_elements.empty() ? 0 : m_elements.back().GetTypeSize());
}
