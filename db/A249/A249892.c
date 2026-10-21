/*
 * A249892  G.f.: Sum_{n>=0} x^n / (1 - n*x - n^2*x^2).
 * https://oeis.org/A249892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} Fibonacci(n-k+1) * k^(n-k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"      /* fib(idx_t) → F[idx_t] (F(0)=0, F(1)=1) */
#include "../utils/big_pow.h"  /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* loop over all k from 0 to n */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* Fibonacci(n-k+1) */
        seq_t fib_val = fib((idx_t)(n - k + 1));

        /* k^(n-k) */
        seq_t pow_val = big_pow((seq_t)k, (seq_t)(n - k));

        total += fib_val * pow_val;
    }

    return total;
}
