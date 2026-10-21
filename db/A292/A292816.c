/*
 * A292816  b(0) = 1, b(2*n-1) =
 *          1/(1+1^2/(1+1^2/(1+2^2/(1+2^2/(...+(n-1)^2/(1+n^2)))))) and b(2*n)
 *          = 1/(1+1^2/(1+1^2/(1+2^2/(1+2^2/(...+n^2/(1+n^2)))))). a(n) is the
 *          numerator of b(n).
 * https://oeis.org/A292816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of the reduced fraction b(n) defined by:
 *            b(0) = 1, for odd n = 2*m-1: b(n) =
 *            1/(1+1²/(1+1²/(…+(m-1)²/(1+m²)))) (each k<m appears twice, m
 *            appears once), for even n = 2*m: b(n) =
 *            1/(1+1²/(1+1²/(…+(m-1)²/(1+m²/(1+m²))))) (each k≤m appears
 *            twice)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* determine m */
    idx_t m;
    int odd = (n & 1ULL) != 0;
    if (odd)
        m = (n + 1) / 2;   /* n = 2*m-1 */
    else
        m = n / 2;         /* n = 2*m */

    /* current value as fraction p/q */
    seq_t p = (seq_t)1 + (seq_t)m * (seq_t)m;  /* 1 + m² */
    seq_t q = (seq_t)1;

    /* even case: one extra application for k = m */
    if (!odd) {
        seq_t kk = (seq_t)m * (seq_t)m;
        seq_t new_p = p + kk * q;
        seq_t new_q = p;
        seq_t g = gcd(new_p, new_q);
        new_p /= g;
        new_q /= g;
        p = new_p;
        q = new_q;
    }

    /* process k = m-1 .. 1, each twice */
    for (idx_t k = m - 1; k >= 1; --k) {
        seq_t kk = (seq_t)k * (seq_t)k;

        /* first application */
        seq_t new_p = p + kk * q;
        seq_t new_q = p;
        seq_t g = gcd(new_p, new_q);
        new_p /= g;
        new_q /= g;
        p = new_p;
        q = new_q;

        /* second application */
        new_p = p + kk * q;
        new_q = p;
        g = gcd(new_p, new_q);
        new_p /= g;
        new_q /= g;
        p = new_p;
        q = new_q;
    }

    /* b(n) = 1 / (p/q) = q / p; return numerator */
    return q;
}
