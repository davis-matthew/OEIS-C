/*
 * A378801  G.f. A(x) satisfies A(x) = ( 1 + x/(1 - x*A(x)^(2/3)) )^3.
 * https://oeis.org/A378801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} 3 * binom(2n-2k+3,k) * binom(n-1,k-1) /
 *            (2n-2k+3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
static inline seq_t mul_div(seq_t val, uint64_t num, uint64_t den) {
    if constexpr (std::is_integral<seq_t>::value && sizeof(seq_t) <= 8) {
        unsigned __int128 prod = (unsigned __int128)val * (unsigned __int128)num;
        return (seq_t)(prod / den);
    } else {
        seq_t prod = val * (seq_t)num;
        return prod / (seq_t)den;
    }
}
#else
static inline seq_t mul_div(seq_t val, uint64_t num, uint64_t den) {
    unsigned __int128 prod = (unsigned __int128)val * (unsigned __int128)num;
    return (seq_t)(prod / den);
}
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t U = 3;          /* first term (k=1)                      */
    seq_t sum = U;        /* accumulate contributions             */
    for (idx_t k = 1; k < n; ++k) {
        uint64_t Ak = 2*n - 2*k + 3;
        /* compute numerator of the ratio R_k */
        uint64_t t1 = (uint64_t)(Ak - k);
        uint64_t t2 = (uint64_t)(Ak - k - 1);
        uint64_t t3 = (uint64_t)(Ak - k - 2);
        uint64_t t4 = (uint64_t)(n - k);
        __uint128_t num128 = (__uint128_t)t1 * t2 * t3 * t4;
        uint64_t numer = (uint64_t)num128;

        /* compute denominator of the ratio R_k */
        uint64_t d1 = (uint64_t)(k + 1);
        uint64_t d2 = (uint64_t)k;
        uint64_t d3 = (uint64_t)(Ak - 1);
        uint64_t d4 = (uint64_t)(Ak - 2);
        __uint128_t den128 = (__uint128_t)d1 * d2 * d3 * d4;
        uint64_t denom = (uint64_t)den128;

        U = mul_div(U, numer, denom);
        sum += U;
    }
    return sum;
}
