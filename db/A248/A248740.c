/*
 * A248740  a(n) = Fibonacci(n) mod 1000.
 * https://oeis.org/A248740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) mod 1000 Recurrence: a(0) = 0, a(1) = 1,
 *            a(n) = (a(n‑1) + a(n‑2)) mod 1000, n > 1
 * Range:     u8 n <= 12, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k),   b = F(k+1), k = n/2 */

    /* all computations are performed modulo 1000 */
    const seq_t MOD = (seq_t)1000;
    seq_t a_mod = a % MOD;
    seq_t b_mod = b % MOD;
    seq_t two_b_mod = (b_mod + b_mod) % MOD;
    seq_t diff = (two_b_mod + MOD - a_mod) % MOD;

    seq_t c = (a_mod * diff) % MOD;                     /* F(2k)   */
    seq_t d = (a_mod * a_mod + b_mod * b_mod) % MOD;     /* F(2k+1) */

    if (n & 1) {  /* n is odd */
        *fn  = d;
        *fn1 = (c + d) % MOD;   /* F(n+1) = F(2k+1) + F(2k) */
    } else {      /* n is even */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    return fn;   /* return Fibonacci(n) mod 1000 */
}
