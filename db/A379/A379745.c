/*
 * A379745  a(n) = Sum_{k=1..n-1} binomial(n,k)*2^(n-k-1)*(k-1).
 * https://oeis.org/A379745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n‑1) - 1)*(n-3)/2 + 2^(n‑1) - 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided power functions pow2 and pow3. */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* For n = 1,2 the formula would underflow an unsigned n‑3,
       but the sequence is 0. */
    if (n < 3) return (seq_t)0;

    /* 3^(n‑1) and 2^(n‑1). */
    seq_t p3 = pow3(n - 1);   /* 3^(n-1) */
    seq_t p2 = pow2(n - 1);   /* 2^(n-1) */

    /* (3^(n-1) - 1). */
    p3 -= (seq_t)1;

    /* (3^(n-1) - 1)*(n-3). */
    seq_t term = p3 * ((seq_t)n - (seq_t)3);

    /* Divide by 2 (integer division guaranteed). */
    term /= (seq_t)2;

    /* Add 2^(n-1) - 1. */
    term += p2;
    term -= (seq_t)1;

    return term;
}
