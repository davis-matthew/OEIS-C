/*
 * A243759  Triangle T(m,k): exponent of the highest power of 3 dividing the
 *          binomial coefficient binomial(m,k).
 * https://oeis.org/A243759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (s₃(k) + s₃(m−k) – s₃(m)) / 2, where m = ⌊(√(8·n+1) –
 *            1)/2⌋, k = n – m·(m+1)/2. (s₃(x) is the sum of base‑3 digits of
 *            x.)
 * Range:     u8 n <= 9279, u16 n <= 9279, u32 n <= 9279, u64 n <= 9279,
 *            u128 n <= 9279, big any n
 * Helpers:   sum3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum3.h"

/* integer square root of an unsigned 64‑bit integer */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    idx_t t = isqrt_u64(8 * n + 1);
    idx_t m = (t - 1) >> 1;          /* (t-1)/2 */
    idx_t offset = m * (m + 1) / 2;
    idx_t k = n - offset;

    unsigned int s_m  = sum3(m);
    unsigned int s_k  = sum3(k);
    unsigned int s_mk = sum3(m - k);

    unsigned int carries = (s_k + s_mk - s_m) / 2;
    return (seq_t)carries;
}
