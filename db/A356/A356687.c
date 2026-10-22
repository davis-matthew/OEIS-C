/*
 * A356687  a(n) = n! * Sum_{k=0..n} k^(2*n)/k!.
 * https://oeis.org/A356687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(2*n) / k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t fact_n = factorial(n);   /* n! */
    seq_t factor = fact_n;         /* factor = n! / k! */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t exp   = (seq_t)(2 * n);
        seq_t pow_val = big_pow(k_seq, exp);      /* k^(2n) */
        seq_t term    = factor * pow_val;          /* (n! / k!) * k^(2n) */
        sum += term;
        if (k == n) break;                          /* last iteration */
        factor /= (seq_t)(k + 1);                   /* update factor = n! / (k+1)! */
    }

    return sum;
}
