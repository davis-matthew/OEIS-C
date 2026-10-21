/*
 * A258321  a(n) = Fibonacci(n) + n*Lucas(n).
 * https://oeis.org/A258321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) + n * Lucas(n)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the fast‑doubling method for Fibonacci, works for all n ≥ 0 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);          /* a = F(k),  b = F(k+1),  k = n / 2 */

    seq_t two_b       = b + b;         /* 2*F(k+1) */
    seq_t two_b_minus_a = two_b - a;   /* 2*F(k+1) – F(k) = F(k-1) + F(k+1) */

    c = a * two_b_minus_a;             /* F(2k)   */
    d = a * a + b * b;                 /* F(2k+1) */

    if ((n & 1) == 0) {                /* even n */
        *fn  = c;
        *fn1 = d;
    } else {                           /* odd n */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);            /* fn  = F(n), fn1 = F(n+1) */

    /* Lucas(n) = 2*F(n+1) – F(n)  (valid for all n ≥ 0) */
    seq_t lucas = (fn1 + fn1) - fn;

    return fn + (seq_t)n * lucas;
}
