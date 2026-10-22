/*
 * A368890  a(n) = Sum_{k=0..floor(n/2)} n^(3*(n-2*k)) * binomial(n-k,k).
 * https://oeis.org/A368890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} n^(3*(n-2*k)) * binomial(n-k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* Offset: 0 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly (a(0)=a(1)=1). */
    if (n <= 1)
        return (seq_t)1;

    /* n as seq_t for intermediate computations. */
    const seq_t sn = (seq_t)n;

    /* Compute n^3 once. */
    const seq_t n3 = sn * sn * sn;

    /* For the recurrence of exponents we need n^6 = (n^3)^2. */
    const seq_t n3sq = n3 * n3;

    /* Start with the term for k = 0: exponent is 3*n. */
    seq_t power = seq_pow(n3, (idx_t)n);

    seq_t sum = (seq_t)0;
    const idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin = binomial(n - k, k);
        sum += power * bin;
        if (k != maxk)
            power /= n3sq;              /* move to exponent for next k */
    }

    return sum;
}
