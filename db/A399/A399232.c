/*
 * A399232  a(n) = Sum_{k=1..4*n+2} (k^(4*n+1) mod (2*n+1)).
 * https://oeis.org/A399232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)^2 - (2n+1)^2 / rad(2n+1) where rad(m) is the
 *            product of the distinct prime factors of m. This follows from
 *            the OEIS comment: rad(2n+1) = (2n+1)^2 / ((2n+1)^2 - a(n)). The
 *            function computes a(n) for n >= 1
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_spf.h"

/* Global smallest prime factor table */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure that spf[i] is defined for all i <= limit */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    if (spf)
        free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Compute rad(m) where m >= 2 */
static seq_t rad_from_spf(idx_t m)
{
    seq_t rad = 1;
    while (m > 1)
    {
        idx_t p = spf[m];
        rad *= (seq_t)p;
        while (m % p == 0)
            m /= p;
    }
    return rad;
}

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) corresponds to n=1 */
    if (n < 1)
        return 0; /* Undefined behaviour for n < 1, but return 0 */

    idx_t m = 2 * n + 1;              /* odd modulus */
    ensure_spf(m);                   /* build or extend spf table up to m */
    seq_t radm = rad_from_spf(m);    /* product of distinct primes dividing m */
    seq_t m2   = (seq_t)m * (seq_t)m;
    seq_t res  = m2 - m2 / radm;     /* a(n) = m^2 - m^2 / rad(m) */
    return res;
}
