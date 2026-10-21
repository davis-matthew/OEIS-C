/*
 * A282079  Number of n-element subsets of [n+2] having an even sum.
 * https://oeis.org/A282079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of n‑element subsets of [n+2] having an even sum.
 *            Computation uses the identity: even(n,N) = ( C(N,n) + D(n,N) ) /
 *            2 where D(n,N) = coefficient of x^n in (1−x)^⌈N/2⌉ (1+x)^⌊N/2⌋
 *            and is evaluated as the alternating sum D = Σ (-1)^a C(⌈N/2⌉,a)
 *            C(⌊N/2⌋, n−a) (0 ≤ a ≤ n)
 * Range:     u8 n <= 14, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Parameters of the problem */
    idx_t N = n + 2;                 /* total elements */
    idx_t O = (N + 1) / 2;           /* number of odd elements in [1..N]   */
    idx_t E = N / 2;                 /* number of even elements           */

    /* Total number of size‑n subsets */
    seq_t total = binomial(N, n);

    /* Accumulators for positive (a even) and negative (a odd) terms */
    seq_t pos = 0, neg = 0;

    /* Upper bound for the summation index a */
    idx_t max_a = (n < O) ? n : O;

    for (idx_t a = 0; a <= max_a; ++a)
    {
        idx_t b = n - a;              /* b is the exponent of (1+x)^E */
        if (b > E)      /* term vanishes if k > number of available evens */
            continue;

        seq_t term = binomial(O, a) * binomial(E, b);
        if ((a & 1ULL) == 0)
            pos += term;
        else
            neg += term;
    }

    /* Compute (total + pos - neg) / 2 without temporary negative values. */
    seq_t sum;
    if (pos >= neg)
        sum = total + (pos - neg);
    else
        sum = total - (neg - pos);

    return sum >> 1;   /* divide by 2 */
}
