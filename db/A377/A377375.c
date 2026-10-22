/*
 * A377375  Antidiagonal sums of A342819.
 * https://oeis.org/A377375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(2*n^2 + 10*n - 3*(1 + (-1)^n)) / 12 for n > 1 a(0)
 *            = a(1) = a(2) = 0 (1 + (-1)^n) is 2 when n is even, 0 when n is
 *            odd
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n < 3u) return (seq_t)0;

    seq_t n_s = (seq_t)n;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    /* Avoid signed overflow: use unsigned ops only */
    seq_t two_n_sq = (seq_t)2 * n_s * n_s;   /* 2*n^2 */
    seq_t ten_n    = (seq_t)10 * n_s;        /* 10*n */
    seq_t term     = two_n_sq + ten_n;       /* 2*n^2 + 10*n */

    /* Compute 1 + (-1)^n : 2 if even, 0 if odd */
    seq_t one_plus_pm2 = ((n_s & (seq_t)1) == 0) ? (seq_t)2 : (seq_t)0;

    seq_t sub = (seq_t)3 * one_plus_pm2;     /* 3*(1+(-1)^n) */
    seq_t inside = term - sub;               /* 2*n^2 + 10*n - 3*(1+(-1)^n) */

    seq_t num = (n_s - (seq_t)2) * inside;   /* (n-2)*(...) */
    seq_t result = num / (seq_t)12;           /* divide by 12 */
    return result;
#else
    /* Fallback for older standards: keep operations in seq_t */
    seq_t two_n_sq = (seq_t)2 * n_s * n_s;
    seq_t ten_n = (seq_t)10 * n_s;
    seq_t term = two_n_sq + ten_n;
    seq_t one_plus_pm2 = ((n_s & (seq_t)1) == 0) ? (seq_t)2 : (seq_t)0;
    seq_t sub = (seq_t)3 * one_plus_pm2;
    seq_t inside = term - sub;
    seq_t num = (n_s - (seq_t)2) * inside;
    seq_t result = num / (seq_t)12;
    return result;
#endif
}
