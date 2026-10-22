/*
 * A319993  a(n) = A319997(n) / A173557(n).
 * https://oeis.org/A319993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = A319997(n) / A173557(n) Multiplicative definition: - a(2)
 *            = −1 - a(2^e) = 0 (e > 1) - a(p^e) = p^(e−1) for odd prime p
 *            Thus for odd n: a(n) = n / rad(n) and for n = 2·m with m odd:
 *            a(n) = − (m / rad(m)). For n divisible by a higher power of 2:
 *            a(n) = 0
 * Range:     i8 n <= 728, i16 n <= 9481, i32 n <= 9481, i64 n <= 9481,
 *            i128 n <= 9481, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t odd_part_over_rad(idx_t m)
/*  Return product of p^(e−1) for the odd integer m.
    Compute by dividing m once by each distinct prime factor. */
{
    uint64_t temp = (uint64_t)m;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        temp /= pr[i];
    }
    return (seq_t)temp;
}

seq_t a(idx_t n)
/*  OEIS A319993 (see formula comment above). Handles any n ≥ 1. */
{
    if (n == 0) return 0;  /* safety for out‑of‑range */

    /* Count power of two in n (trailing zero bits). */
    idx_t tz = 0;
    idx_t tmp = n;
    while ((tmp & (idx_t)1) == 0) {
        ++tz;
        tmp >>= 1;
    }

    if (tz > 1)                 /* 2² | n → a(n) = 0 */
        return 0;

    idx_t odd = (tz == 0) ? n : (n >> 1);
    seq_t val = odd_part_over_rad(odd);

    if (tz == 1)                /* n = 2·odd → a(n) = −val */
        val = -val;

    return val;
}
