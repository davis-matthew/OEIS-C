/*
 * A302611  Expansion of e.g.f. -log(1 - x)*arctanh(x).
 * https://oeis.org/A302611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302611 – Expansion of -log(1 - x)*arctanh(x) */
/* a(0)=0, a(1)=0, a(n)= n! * Σ_{odd i<n} 1/(i*(n-i)) */
/*  For odd n=2m+1 : a(n)= (2m)! * H_{2m} */
/*  For even n=2m   : a(n)= (2m)! * (H_{2m-1} - 0.5 H_{m-1}) / m */
/*  where H_k is the k‑th harmonic number.           */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    /* odd n = 2m+1 ----------------------------------------------------- */
    if (n & 1ULL) {
        seq_t fact = 1;
        for (idx_t i = 2; i <= n - 1; ++i) {
            fact *= (seq_t)i;          /* (n-1)! */
        }

        seq_t sum = 0;
        for (idx_t i = 1; i <= n - 1; ++i) {
            sum += fact / (seq_t)i;    /* (n-1)! / i  */
        }
        return sum;                     /* a(n) = Σ (n-1)!/i */
    }

    /* even n = 2m ------------------------------------------------------- */
    idx_t m = n / 2;                    /* m = n/2 */

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;                /* n!   */
    }

    seq_t N1 = 0;                       /* Σ_{i=1}^{n-1} n!/i */
    for (idx_t i = 1; i <= n - 1; ++i) {
        N1 += fact / (seq_t)i;
    }

    seq_t N2 = 0;                       /* Σ_{i=1}^{m-1} n!/i  */
    for (idx_t i = 1; i <= m - 1; ++i) {
        N2 += fact / (seq_t)i;
    }

    seq_t numerator   = N1 * 2 - N2;     /* 2*N1 - N2   */
    seq_t denominator = (seq_t)(2 * m); /* 2*m         */

    return numerator / denominator;     /* exact integer result */
}
