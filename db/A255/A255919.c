/*
 * A255919  Gray code of Fibonacci(n).
 * https://oeis.org/A255919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n) ^ (F(n) >> 1) where F(n) is the n-th Fibonacci
 *            number with F(0)=0, F(1)=1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    /* Returns (F(n),F(n+1)) using fast‑doubling */
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = b + b;
    seq_t c     = a * (two_b - a);     /* F(2k)   */
    seq_t d     = a * a + b * b;       /* F(2k+1) */

    if ((n & 1) == 0) {                /* n even */
        *fn  = c;
        *fn1 = d;
    } else {                           /* n odd  */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);            /* fn = F(n)   */
    seq_t half = fn >> 1;              /* right‑shift by one bit */
    return fn ^ half;                  /* Gray code of F(n) */
}
