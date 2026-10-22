/*
 * A346957  a(0) = 0, a(1) = 1, and b = a(n-2) + a(n-1). a(n) = b if b is a
 *          prime; otherwise a(n) = a(n-1) and a(n-1) = b.
 * https://oeis.org/A346957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 32, u32 n <= 35, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A346957: a(0)=0, a(1)=1.
   For n≥2, compute b = a(n-2)+a(n-1).
   If b is prime then a(n)=b; otherwise
   a(n)=a(n-1) and a(n-1)=b.
   Due to possible overwriting of a(n-1) on composite steps,
   the final value of a(k) is stable only after processing step k+1. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0; /* a(i-2) */
    seq_t prev1 = (seq_t)1; /* a(i-1) */

    /* Process steps i = 2 .. n+1 inclusive.  After finishing the loop,
       prev2 holds a(n) (the final value after possible overwrites). */
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t b = prev2 + prev1;
        bool prime = is_prime_big(b);

        seq_t ai;
        seq_t new_prev2, new_prev1;

        if (prime) {
            ai = b;
            new_prev2 = prev1;   /* a(i-1) stays the same */
            new_prev1 = ai;      /* a(i) becomes b */
        } else {
            ai = prev1;          /* a(i) stays old a(i-1) */
            new_prev2 = b;       /* a(i-1) becomes b */
            new_prev1 = prev1;   /* a(i) remains old a(i-1) */
        }

        prev2 = new_prev2;
        prev1 = new_prev1;
    }

    return prev2; /* a(n) */
}
