/*
 * A333125  a(n) = binomial(Fibonacci(n),n).
 * https://oeis.org/A333125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( fib(n), n )
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>
#include "../utils/fib.h"

/* Return the binomial coefficient C(F, n) for arbitrary precision
   type seq_t using the multiplicative formula.
   The product is always integer at every step, so division is exact. */
static seq_t binom_seq(seq_t F, idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t result = (seq_t)1;
    /* Use the symmetry property to keep the number of multiplications
       minimal if n > F/2, but since n ≤ F for this sequence,
       we compute directly. */
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* Compute (F - k + 1) as a seq_t */
        seq_t numerator = F - (seq_t)(k - (idx_t)1);
        result = (result * numerator) / (seq_t)k;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Fibonacci numbers: fib(0)=0, fib(1)=1, fib(2)=1, ... */
    seq_t F = fib(n);
    /* If n > F, the binomial coefficient is zero */
    if ((seq_t)n > F)
        return (seq_t)0;
    return binom_seq(F, n);
}
