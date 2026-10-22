/*
 * A376919  The nonpowers of 3.
 * https://oeis.org/A376919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 For n > 0: let p = floor(log_3(n)) let t = 3^(p+1) a(n)
 *            = n + p + 1 + (n + p + 1 >= t ? 1 : 0)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* required by the build system, not used otherwise */

static inline idx_t floor_log3(idx_t n)
{
    idx_t k = 0;
    while (n >= 3u) {
        n /= 3u;
        k++;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0u;

    idx_t p = floor_log3(n);

    /* Compute t = 3^(p+1) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 t = 1;
    for (idx_t i = 0; i < p + 1; ++i)
        t *= 3u;

    unsigned __int128 lhs = (unsigned __int128)n + p + 1u;
    seq_t rn = (seq_t)n;

    return rn + p + 1u + (lhs >= t ? (seq_t)1u : (seq_t)0u);
}
