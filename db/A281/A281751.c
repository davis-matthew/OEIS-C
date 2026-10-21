/*
 * A281751  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 395", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A281751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=7, a(3)=2, a(4)=23, a(5)=58, a(6)=23,
 *            a(7)=170, a(8)=471, a(9)=186, a(10)=1367, a(11)=3818,
 *            a(12)=1367, a(13)=11194, a(14)=30039, a(15)=12010, a(16)=87383,
 *            a(17)=244410, a(18)=87383, a(19)=715498. For n>19: a(n) = -
 *            2*a(n-1) + 8*a(n-3) + 16*a(n-4) + a(n-8) + 2*a(n-9) - 8*a(n-...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* small multiplication by a small integer constant */
static inline seq_t mul_by_small(seq_t x, unsigned int m) {
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)((__int128)(x) * m);
    } else {
        return x * (seq_t)m;
    }
#else
    /* In C we assume seq_t is an integral type */
    return (seq_t)((__int128)(x) * m);
#endif
}

static const seq_t init_vals[20] = {
    1ULL, 2ULL, 7ULL, 2ULL, 23ULL, 58ULL, 23ULL, 170ULL,
    471ULL, 186ULL, 1367ULL, 3818ULL, 1367ULL, 11194ULL,
    30039ULL, 12010ULL, 87383ULL, 244410ULL, 87383ULL, 715498ULL
};

seq_t a(idx_t n) {
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        /* Integral types: use __int128 for all intermediate arithmetic */
        if (n < 20) return init_vals[(size_t)n];
        __int128 buf[20];
        for (size_t i = 0; i < 20; ++i)
            buf[i] = (__int128)init_vals[i];
        for (idx_t i = 20; i <= n; ++i) {
            size_t im1  = (size_t)((i - 1) % 20);
            size_t im3  = (size_t)((i - 3) % 20);
            size_t im4  = (size_t)((i - 4) % 20);
            size_t im8  = (size_t)((i - 8) % 20);
            size_t im9  = (size_t)((i - 9) % 20);
            size_t im11 = (size_t)((i - 11) % 20);
            size_t im12 = (size_t)((i - 12) % 20);
            __int128 pos = buf[im3] * 8 + buf[im4] * 16 + buf[im8] + buf[im9] * 2;
            __int128 neg = buf[im1] * 2 + buf[im11] * 8 + buf[im12] * 16;
            buf[(size_t)i % 20] = pos - neg;
        }
        return (seq_t)buf[(size_t)n % 20];
    } else {
        /* Non-integral types (arbitrary‑precision): use seq_t arithmetic */
        if (n < 20) return init_vals[(size_t)n];
        seq_t ring[20];
        for (size_t i = 0; i < 20; ++i) ring[i] = init_vals[i];
        for (idx_t i = 20; i <= n; ++i) {
            size_t im1  = (size_t)((i - 1) % 20);
            size_t im3  = (size_t)((i - 3) % 20);
            size_t im4  = (size_t)((i - 4) % 20);
            size_t im8  = (size_t)((i - 8) % 20);
            size_t im9  = (size_t)((i - 9) % 20);
            size_t im11 = (size_t)((i - 11) % 20);
            size_t im12 = (size_t)((i - 12) % 20);
            seq_t pos = mul_by_small(ring[im3], 8);
            pos = pos + mul_by_small(ring[im4], 16);
            pos = pos + ring[im8];
            pos = pos + mul_by_small(ring[im9], 2);
            seq_t neg = mul_by_small(ring[im1], 2);
            neg = neg + mul_by_small(ring[im11], 8);
            neg = neg + mul_by_small(ring[im12], 16);
            ring[(size_t)i % 20] = pos - neg;
        }
        return ring[(size_t)n % 20];
    }
#else
    /* Pure C implementation: seq_t is an integral type */
    if (n < 20) return init_vals[(size_t)n];
    __int128 buf[20];
    for (size_t i = 0; i < 20; ++i)
        buf[i] = (__int128)init_vals[i];
    for (idx_t i = 20; i <= n; ++i) {
        size_t im1  = (size_t)((i - 1) % 20);
        size_t im3  = (size_t)((i - 3) % 20);
        size_t im4  = (size_t)((i - 4) % 20);
        size_t im8  = (size_t)((i - 8) % 20);
        size_t im9  = (size_t)((i - 9) % 20);
        size_t im11 = (size_t)((i - 11) % 20);
        size_t im12 = (size_t)((i - 12) % 20);
        __int128 pos = buf[im3] * 8 + buf[im4] * 16 + buf[im8] + buf[im9] * 2;
        __int128 neg = buf[im1] * 2 + buf[im11] * 8 + buf[im12] * 16;
        buf[(size_t)i % 20] = pos - neg;
    }
    return (seq_t)buf[(size_t)n % 20];
#endif
}
