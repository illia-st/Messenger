#pragma once

#include <cstddef>
#include <string>

namespace TCP{
    constexpr size_t MaxBufferSize = 1500;
    enum class Flag: uint8_t{
        DefaultMsg,
        MessengerMsg
    };
    namespace {
        template<size_t N = MaxBufferSize>
        class HiddenBuffer final {
        public:
            // To avoid memory leak you should use Clear function
            // Sucb a desision was made to let Buffer be used in unions
            HiddenBuffer() = default;

            void SetSize(size_t length);

            void SetInfo(const char *arr);

            void SetFlag(uint8_t flag);

            char *GetInfo();

            const char *GetInfo() const;

            size_t GetSize() const;

            uint8_t GetFlag() const;

            void Clear();

        private:
            uint8_t m_flag{};
            size_t size{};
            char chr[N];
        };

        template<size_t N>
        void HiddenBuffer<N>::SetSize(size_t length) { size = length; }

        template<size_t N>
        void HiddenBuffer<N>::SetInfo(const char *arr) { chr = const_cast<char *>(arr); }

        template<size_t N>
        char *HiddenBuffer<N>::GetInfo() { return chr; }

        template<size_t N>
        void HiddenBuffer<N>::SetFlag(uint8_t flag) { m_flag = flag; }

        template<size_t N>
        const char *HiddenBuffer<N>::GetInfo() const { return chr; }

        template<size_t N>
        size_t HiddenBuffer<N>::GetSize() const { return size; }

        template<size_t N>
        uint8_t HiddenBuffer<N>::GetFlag() const { return m_flag; }

        template<size_t N>
        void HiddenBuffer<N>::Clear() {
            size = 0;
        }
    }
    template<size_t N>
    union Buffer{
        HiddenBuffer<N> buf;
        char bytes[sizeof(HiddenBuffer<N>)];
    };
}
