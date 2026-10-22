/*
 * A378277  Denominators in a harmonic triangle, based on products of
 *          Fibonacci numbers.
 * https://oeis.org/A378277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = T(r,k) where T(r,k) = F_r * F_{r+1} if k==r, T(r,k) = F_k
 *            * F_{k+2} if 1 <= k < r. r is the unique integer with r(r+1)/2
 *            >= n, prev row sum prev_cum = (r-1)r/2, k = n - prev_cum
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Fast doubling: compute F(n) and F(n+1) into *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);              /* a = F(k), b = F(k+1), k = n/2 */

    seq_t c = a * ((b << 1) - a);          /* F(2k) = F(k)*(2*F(k+1)-F(k)) */
    seq_t d = a * a + b * b;               /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if (n & 1) {                           /* n odd: use 2k+1 formulas */
        *fn  = d;
        *fn1 = c + d;              /* F(n+1) = F(2k+2) */
    } else {                               /* n even: use 2k formulas */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;                  /* a(1) = 1 */

    /* Find the row r such that cumulative count >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t cum = (mid * (mid + 1)) >> 1;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev_cum = ((r - 1) * r) >> 1;
    idx_t k = n - prev_cum;                /* column index (1‑based) */

    if (k == r) {                          /* main diagonal */
        seq_t fr, fr1;
        fib_pair(r, &fr, &fr1);
        return fr * fr1;
    } else {                               /* off diagonal */
        seq_t fk, fk1, fk2;
        fib_pair(k, &fk, &fk1);            /* fk = F(k), fk1 = F(k+1) */
        fib_pair(k + 1, &fk1, &fk2);       /* fk2 = F(k+2) */
        return fk * fk2;
    }
}
