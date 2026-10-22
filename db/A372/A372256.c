/*
 * A372256  a(n) = (n-1)!/2^floor((n-1)/2) + floor((n-1)/2).
 * https://oeis.org/A372256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! / 2^floor((n-1)/2) + floor((n-1)/2)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (n ≥ 1; offset = 1)                                           */

#include <stdint.h>   /* only for int types used below */

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset = 1) */
    idx_t m = n - 1;                 /* compute (n-1)!   */
    idx_t k = m / 2;                 /* floor((n-1)/2)   */

    seq_t result = 1;
    idx_t remaining = k;             /* number of 2‑factors still to cancel */

    for (idx_t i = 1; i <= m; ++i)
    {
        seq_t term = (seq_t)i;

        /* remove as many 2‑factors as possible from the current term */
        while (remaining && !(term & 1))     /* term even */
        {
            term >>= 1;
            --remaining;
        }

        result *= term;

        /* cancel any remaining 2‑factors from the running product */
        while (remaining && !(result & 1))   /* result even */
        {
            result >>= 1;
            --remaining;
        }
    }

    /* if any 2‑factors remain, cancel them (should be none for valid data) */
    if (remaining)
        result >>= remaining;

    /* add the floor((n-1)/2) term */
    result += (seq_t)k;

    return result;
}
