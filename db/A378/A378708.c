/*
 * A378708  Circumference of the n-Lucas cube graph.
 * https://oeis.org/A378708
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Lucas(n) - (n mod 3 == 0 ? 2 : 1); a(4) = 4
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);

    seq_t t = b * 2;
    t = t - a;
    seq_t c = a * t;
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        seq_t e = c + d;
        *fn1 = e;
    }
}

seq_t a(idx_t n)
{
    if (n == 4)
        return 4;

    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);

    seq_t l = fn1 * 2 - fn;        /* Lucas(n) */
    if ((n % 3) == 0)
        l = l - 2;
    else
        l = l - 1;
    return l;
}
