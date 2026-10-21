/*
 * A292465  a(n) = n*F(n)*F(n+1), where F = A000045.
 * https://oeis.org/A292465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * F(n) * F(n+1) (F is Fibonacci, F(0)=0, F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* (a, b) = (F(k), F(k+1)) for k = n/2 */

    /* c = a * (2*b - a)  ; d = a*a + b*b  */
    seq_t two_b = b << 1;
    seq_t c = a * (two_b - a);
    seq_t d = a * a + b * b;

    if ((n & 1) == 0)
    {
        *fn  = c;
        *fn1 = d;
    }
    else
    {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    return ((seq_t)n) * fn * fn1;
}
