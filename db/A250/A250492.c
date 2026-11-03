/*
 * A250492  (n^6)-th Fibonacci number.
 * https://oeis.org/A250492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: standard, pass, 5 terms through n = 4 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Fibonacci(n^6)
 */

#include <stdint.h>

/* Fast doubling Fibonacci for seq_t indices.  This works for any
 * integer type that overloads +, -, *, /, % and comparison operators,
 * including built‑in integer types and arbitrary‑precision integer
 * classes that support these operators. */
static void fib_pair_seq(seq_t k, seq_t *fn, seq_t *fn1)
{
    if (k == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }

    seq_t half = k / 2;
    seq_t a, b;
    fib_pair_seq(half, &a, &b);

    seq_t two_b = b * 2;
    seq_t c = a * (two_b - a);           /* F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    seq_t d = a * a + b * b;             /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((k % 2) == 0) {                  /* even index */
        *fn  = c;
        *fn1 = d;
    } else {                             /* odd index */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Main sequence function.  Computes a(n) = Fibonacci(n^6). */
seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;        /* n^2   */
    seq_t nby4 = nsq * nsq;                /* n^4   */
    seq_t idx = nby4 * nsq;                 /* n^6   */

    seq_t fn, fn1;
    fib_pair_seq(idx, &fn, &fn1);
    return fn;
}
