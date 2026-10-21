/*
 * A279055  Self-convolution of squares of factorial numbers (A001044).
 * https://oeis.org/A279055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} (i! * (n-i)!)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

/* Computes a(0)=1, a(1)=2, ... */
seq_t a(idx_t n)
{
    /* Compute factorial(n) */
    seq_t fact = 1;
    for (idx_t k = 1; k <= n; ++k) {
        fact = mul_small(fact, (unsigned int)k);   /* fact *= k */
    }

    /* T = i! * (n-i)! for i = 0 (initially 0! * n! = fact) */
    seq_t T = fact;

    /* sum = T^2 for i = 0 */
    seq_t sum = seq_mul(T, T);

    /* Iterate i = 0 .. n-1 updating T = (i+1)! * (n-i-1)! */
    for (idx_t i = 0; i < n; ++i) {
        /* T = T * (i+1) / (n-i)  */
        T = mul_small(T, (unsigned int)(i + 1));   /* T *= (i+1) */
        T = T / (seq_t)(n - i);                    /* exact division */

        /* Add square of new T to the sum */
        seq_t term = seq_mul(T, T);
        sum += term;
    }

    return sum;
}
