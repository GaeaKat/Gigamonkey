// Copyright (c) 2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef GIGAMONKEY_HASH
#define GIGAMONKEY_HASH

#include "types.hpp"

#include "arith_uint256.h"

namespace Gigamonkey {
    
    template <unsigned int size, unsigned int bits = 8 * size> struct uint : base_uint<bits> {
        uint(uint64); // TODO
        uint() : uint(0) {}
        
        uint(slice<size>);
        
        explicit uint(string_view hex);
        
        byte* begin() {
            return (byte*)base_uint<bits>::pn;
        }
        
        byte* end() {
            return begin() + base_uint<bits>::WIDTH;
        }
        
        const byte* begin() const {
            return (byte*)this->pn;
        }
        
        const byte* end() const {
            return begin() + base_uint<bits>::WIDTH;
        }
        
        byte* data() {
            return begin();
        }
        
        const byte* data() const {
            return begin();
        }
        
        operator slice<size>();
        
        operator const slice<size>() const;
        
        operator bytes_view() const {
            return bytes_view{data(), base_uint<bits>::WIDTH};
        }
        
        operator N() const;
    };
    
    using uint256 = uint<32>;
    using uint160 = uint<20>;
    
    template <unsigned int size> struct digest : nonzero<uint<size>> {
        
        digest() : nonzero<uint<size>>{} {}
        
        explicit digest(const uint<size>& u) : nonzero<uint<size>>{u} {}
        explicit digest(string_view s) : nonzero<uint<size>>{uint<size>{s}} {}
        explicit digest(const nonzero<uint32_little>& n) : nonzero<uint32_little>{n} {}
        explicit digest(const slice<size>& x) : digest{uint<size>(x)} {}
        explicit digest(const base_uint<size * 8>& b);
        
        operator bytes_view() const {
            return bytes_view(nonzero<uint<size>>::Value);
        }
        
        explicit operator N() const;
    };

    using digest160 = digest<20>;
    using digest256 = digest<32>;
    using digest512 = digest<64>;
    
    digest160 ripemd160(bytes_view b);
    digest256 sha256(bytes_view b);
    
    digest160 ripemd160(string_view b);
    digest256 sha256(string_view b);
    
    namespace Bitcoin {
    
        digest160 hash160(bytes_view b);
        digest256 hash256(bytes_view b);
    
        digest160 hash160(string_view b);
        digest256 hash256(string_view b);
        
        inline digest<20> address_hash(bytes_view b) {
            return hash160(b);
        }
        
        inline digest<32> signature_hash(bytes_view b) {
            return hash256(b);
        }
    
    }
    
}

template <size_t size> 
inline std::ostream& operator<<(std::ostream& o, const Gigamonkey::digest<size>& s) {
    return o << "digest{" << s.Digest << "}";
}

template <unsigned int size, unsigned int bits> 
inline Gigamonkey::bytes_writer operator<<(Gigamonkey::bytes_writer w, const Gigamonkey::uint<size, bits>& s) {
    return w << data::bytes_view(s);
}

template <unsigned int size, unsigned int bits>
inline Gigamonkey::bytes_reader operator>>(Gigamonkey::bytes_reader r, Gigamonkey::uint<size, bits>& s) {
    data::bytes b(size);
    r >> b;
    std::copy(b.begin(), b.end(), s.begin());
    return r;
}

template <unsigned int size> 
inline Gigamonkey::bytes_writer operator<<(Gigamonkey::bytes_writer w, const Gigamonkey::digest<size>& s) {
    return w << s.Value;
}

template <unsigned int size> 
inline Gigamonkey::bytes_reader operator>>(Gigamonkey::bytes_reader r, Gigamonkey::digest<size>& s) {
    return r >> s.Value;
}

namespace Gigamonkey {
    
    template <unsigned int size, unsigned int bits>
    uint<size, bits>::uint(uint64 u) {
        uint64_little x = u;
        if (size < 8) {
            std::copy(x.begin() + 8 - size, x.end(), begin());
        } else {
            for (auto x = begin(); x != end() - 8; x++) *x = 0;
            std::copy(x.begin(), x.end(), begin() + size - 8);
        }
    }

    template <unsigned int size, unsigned int bits>
    inline uint<size, bits>::uint(slice<size> x) {
        std::copy(x.begin(), x.end(), begin());
    }
    
    template <unsigned int size, unsigned int bits>
    inline uint<size, bits>::operator N() const {
        data::math::number::bounded<false, data::endian::little, size> n{};
        std::copy(begin(), end(), n.begin());
        return N{n};
    }
    
    
    template <unsigned int size, unsigned int bits>
    inline uint<size, bits>::uint(string_view hex) : uint(0) {
        bytes b(data::encoding::hexidecimal::read(hex, endian::little));
        if (b.size() == size) std::copy(b.begin(), b.end(), begin());
    }

}

#endif
