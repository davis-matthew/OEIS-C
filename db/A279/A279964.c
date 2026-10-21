/*
 * A279964  a(n) = Sum_{k=1..n-1} sigma_3(k)*sigma_7(n-k).
 * https://oeis.org/A279964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} sigma_3(k)*sigma_7(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 107, u128 n <= 1000,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"

static inline seq_t mul_seq(seq_t a, seq_t b)
{
#if defined(__cplusplus)
    if constexpr (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)tmp;
    } else {
        return a * b;
    }
#else
    /* C mode – use 128‑bit intermediate for unsigned types */
    unsigned __int128 tmp = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)tmp;
#endif
}

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)0;   /* a(1) = 0 */

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t s3 = sigma_k(k, 3);
        seq_t s7 = sigma_k(n - k, 7);
        sum += mul_seq(s3, s7);
    }
    return sum;
}
