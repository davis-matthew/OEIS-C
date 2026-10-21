/*
 * A269803  a(n) = F(n+1)*F(n+2) - F(n), where F = A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A269803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1)*F(n+2) - F(n) (F = Fibonacci numbers)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses fast doubling to compute Fibonacci numbers in O(log n). */

static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    /* Compute fib(n/2) and fib(n/2+1) */
    seq_t a, b;
    fib_fast(n >> 1, &a, &b);

    /* Fast doubling formulas */
    seq_t two_b_minus_a = (b << 1) - a;        /* 2*F(k+1) - F(k) */
    seq_t c = a * two_b_minus_a;              /* F(2k)   */
    seq_t d = a * a + b * b;                  /* F(2k+1) */

    if ((n & 1) == 0) {
        /* n is even: F(n)=c,  F(n+1)=d */
        *fn  = c;
        *fn1 = d;
    } else {
        /* n is odd : F(n)=d,  F(n+1)=c+d */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by assumption */
    seq_t fn, fn1, fn2;

    /* Compute F(n) and F(n+1) */
    fib_fast(n, &fn, &fn1);

    /* F(n+2) = F(n) + F(n+1) */
    fn2 = fn + fn1;

    /* a(n) = F(n+1)*F(n+2) - F(n) */
    return fn1 * fn2 - fn;
}
