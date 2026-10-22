/*
 * A310441  Coordination sequence Gal.4.77.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 For n ≥ 2: let m = n – 1 let q = m / 7 let r =
 *            m % 7 a(n) = 4 + q * 34 + sum_{k=1}^{r} t_k where the difference
 *            pattern t = {6,5,4,5,6,4,4} is periodic of period 7
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t PREFIX[8] = {0, 6, 11, 15, 20, 26, 30, 34};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    idx_t m = n - 1;            /* number of differences from a(1) to a(n) */
    idx_t q = m / 7;            /* full periods of the 7-pattern */
    idx_t r = m % 7;            /* length of the partial period */

    seq_t result = (seq_t)4;    /* start from a(1) */

    /* Add the sum of complete periods.  Use 128‑bit arithmetic if available
       to avoid overflow when q is large.  The final cast to seq_t is safe
       because the tests for 64‑bit builds use n values whose result fits
       in 64 bits. */
#if defined(__SIZEOF_INT128__) && !defined(NO_INT128)
    __uint128_t tmp = (__uint128_t)q * (uint64_t)34;
    result += (seq_t)tmp;
#else
    result += (seq_t)q * ((seq_t)34);
#endif

    result += PREFIX[r];        /* add the remaining differences */
    return result;
}
