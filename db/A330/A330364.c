/*
 * A330364  Values of abs(P(x)), with P(x) = (1/72)*x^6 + (1/24)*x^5 -
 *          (1583/72)*x^4 - (3161/24)*x^3 + (200807/36)*x^2 + (97973/3)*x -
 *          11351, for -45 <= x <= 12, sorted by size. All values in the given
 *          range are prime.
 * https://oeis.org/A330364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: precompute P(x) for integer x in [-45,12],
   take absolute values, sort, and return the n‑th smallest. */
#include <stdlib.h>
#include <stdint.h>

/* Helper comparison for qsort on seq_t values. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* There are exactly 58 values for x ∈ [‑45,12]. */
    static seq_t values[58];
    static int initialized = 0;

    if (!initialized)
    {
        size_t count = 0;
        for (int x = -45; x <= 12; ++x)
        {
            /* Use a type wide enough for intermediate calculations. */
            __int128 sx = (__int128)x;

            /* Compute Q(x) = 72 * P(x); Q(x) is an integer. */
            __int128 q =
                sx * sx * sx * sx * sx * sx               /* x⁶ */
              + (__int128)3 * sx * sx * sx * sx * sx       /* +3x⁵ */
              - (__int128)1583 * sx * sx * sx * sx        /* –1583x⁴ */
              - (__int128)9483 * sx * sx * sx             /* –9483x³ */
              + (__int128)401614 * sx * sx               /* +401614x² */
              + (__int128)2351352 * sx                   /* +2351352x */
              - (__int128)817272;                        /* –817272 */

            if (q < 0) q = -q;                                   /* |Q(x)| */
            __int128 val = q / (__int128)72;                    /* P(x) integer */

            values[count++] = (seq_t)val;                       /* cast to seq_t */
        }
        /* sort the 58 values */
        qsort(values, count, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    if (n == 0 || n > 58)
        return (seq_t)0;                /* undefined for out‑of‑range */

    return values[n - 1];
}
