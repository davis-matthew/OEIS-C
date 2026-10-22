/*
 * A381255  Positive integers not of the form round(2^(k+2)/5). Complement of
 *          A007910.
 * https://oeis.org/A381255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)= m+1 if m>=floor((2^k+2)/5), else m, where
 *            k=floor(log2(5*n+3))+1, m=n-2+k. The sequence consists of
 *            positive integers not of the form round(2^(k+2)/5)
 * Range:     u8 n <= 24, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: pow2(x) is provided by the build utilities. */
#include <stdint.h>
#include "../utils/pow2.h"

/* Compute floor(log2(x)) for a 128‑bit unsigned integer. */
static inline unsigned int floor_log2_uint128(unsigned __int128 x)
{
    /* x > 0 */
    if (x <= UINT64_MAX) {
        return 63U - __builtin_clzll((uint64_t)x);
    }
    uint64_t high = (uint64_t)(x >> 64);
    return 127U - __builtin_clzll(high);
}

seq_t a(idx_t n)
{
    /* v = 5*n + 3   (computed in 128‑bit to avoid overflow) */
    unsigned __int128 v = (unsigned __int128)5 * (unsigned __int128)n + 3;
    unsigned int k = floor_log2_uint128(v) + 1;              /* k = floor(log2(5n+3)) + 1 */
    idx_t m = n - 2 + k;                                     /* m = n - 2 + k */

    /* threshold = floor((2^k + 2) / 5) */
    seq_t th = (pow2(k) + 2) / 5;

    seq_t m_seq = (seq_t)m;
    if (m_seq >= th) {
        return m_seq + 1;
    } else {
        return m_seq;
    }
}
