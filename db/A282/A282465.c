/*
 * A282465  a(n) = 11*Fibonacci(n+3) + Fibonacci(n-8) with n>=0.
 * https://oeis.org/A282465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45*Fibonacci(n) + Fibonacci(n+1) a(0) = 1, a(1) = 46,
 *            a(2) = 47,
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 4240 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void _fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {               /* F(0) = 0 , F(1) = 1 */
        *fn = 0;
        *fn1 = 1;
        return;
    }

    /* compute F(k) and F(k+1) where k = n/2 */
    seq_t a, b;
    _fib_pair(n >> 1, &a, &b);

    /* 2*F(k+1) - F(k)  (always non‑negative) */
    seq_t sa = (b << 1) - a;
    /* F(2k)  = F(k) * (2*F(k+1) - F(k)) */
    seq_t c = a * sa;
    /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    _fib_pair(n, &fn, &fn1);
    return 45 * fn + fn1;
}
