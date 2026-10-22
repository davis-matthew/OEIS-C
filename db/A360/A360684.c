/*
 * A360684  Expansion of Sum_{k>=0} (x * (1 + k^2 * x))^k.
 * https://oeis.org/A360684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n-k)^(2*k) * binomial(n-k,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are defined by the build: seq_t may be a native
 * unsigned integer or an arbitrary‑precision integer class.  The function
 * uses only integer operations, the utility binomial() and big_pow()
 * functions, and no floating point or external I/O. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle n=0 explicitly to avoid an empty loop when maxk=0. */
    if (n == 0) return (seq_t)1;

    idx_t maxk = n / 2;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* u = n - k */
        idx_t u = n - k;

        /* Compute u^(2*k) using big_pow with seq_t arguments. */
        seq_t power = big_pow((seq_t)u, (seq_t)(2 * k));

        /* Binomial coefficient C(u, k). */
        seq_t binom = binomial(u, k);

        /* term = power * binom */
        seq_t term = power * binom;

        sum += term;
    }

    return sum;
}
