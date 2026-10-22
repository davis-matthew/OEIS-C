/*
 * A343543  a(n) = n*Lucas(2*n).
 * https://oeis.org/A343543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Lucas(2n), Lucas(2n) = 2*F(2n+1) - F(2n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* fast‑doubling Fibonacci pair for arbitrary seq_t */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;          /* F(0)  */
    seq_t b = 1;          /* F(1)  */

    if (n == 0)
    {
        *fn  = a;
        *fn1 = b;
        return;
    }

    /* find highest set bit in n */
    idx_t mask = 1ULL << (63 - __builtin_clzll(n));

    for (; mask; mask >>= 1)
    {
        /* c = F(2k)   = a * (2*b - a) */
        seq_t c = a * (((seq_t)2 * b) - a);
        /* d = F(2k+1) = a*a + b*b     */
        seq_t d = a * a + b * b;

        if (n & mask)       /* odd index */
        {
            a = d;          /* F(2k+1) */
            b = c + d;      /* F(2k+2) */
        }
        else                /* even index */
        {
            a = c;          /* F(2k)   */
            b = d;          /* F(2k+1) */
        }
    }

    *fn  = a;   /* F(n)   */
    *fn1 = b;   /* F(n+1) */
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    idx_t m = n << 1;          /* m = 2n */
    fib_pair(m, &fn, &fn1);    /* fn  = F(2n)   */
    /* fn1 = F(2n+1) */

    seq_t lucas = (seq_t)2 * fn1 - fn;   /* Lucas(2n) = 2*F(2n+1) - F(2n) */
    return (seq_t)n * lucas;             /* a(n) = n * Lucas(2n) */
}
