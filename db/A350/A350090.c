/*
 * A350090  a(n) is the number of indices i in the range 0 <= i <= n-1 such
 *          that A003215(n) - A003215(i) is an oblong number (A002378), where
 *          A003215 are the hex numbers.
 * https://oeis.org/A350090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { i | 0 <= i < n and 3*n*(n+1) - 3*i*(i+1) is an oblong
 *            number k*(k+1) }
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 8089, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline bool is_oblong(seq_t d)
{
    /* d > 0 */
    seq_t tmp = d * 4 + 1;          /* 1 + 4*d   */
    seq_t s   = floor_sqrt(tmp);    /* floor(sqrt(1+4*d)) */
    if (s * s != tmp)
        return false;               /* discriminant not a square → not oblong */

    seq_t k = (s - 1) / 2;
    return k * (k + 1) == d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sn  = (seq_t)n;
    seq_t hex_n = 3 * sn * (sn + 1);      /* 3*n*(n+1) */

    idx_t cnt = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t si   = (seq_t)i;
        seq_t hex_i = 3 * si * (si + 1);   /* 3*i*(i+1) */
        seq_t diff  = hex_n - hex_i;
        if (is_oblong(diff))
            ++cnt;
    }
    return (seq_t)cnt;
}
