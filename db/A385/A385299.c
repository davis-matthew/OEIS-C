/*
 * A385299  Number of Schröder paths of semilength 2n and having n valleys.
 * https://oeis.org/A385299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (k+1)/(2*n-k)*C(2*n-k,n)*C(3*n-k,2*n+1)
 *            for n>=1, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: binomial(n,k) computed multiplicatively.  Uses gcd to keep intermediate values smaller. */

#include "../utils/gcd.h"

static seq_t binom_mul(idx_t a, idx_t b)
{
    if (b > a - b)
        b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i)
    {
        res = res * (seq_t)(a - b + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t two_n = 2 * n;
    idx_t three_n = 3 * n;

    seq_t sum = 0;
    seq_t B = binom_mul(two_n, n);          /* C(2n, n)           */
    seq_t D = binom_mul(three_n, two_n + 1); /* C(3n, 2n+1)         */

    for (idx_t k = 0; k < n; ++k)
    {
        seq_t denom = (seq_t)(two_n - k);

        seq_t g1 = gcd(B, denom);
        seq_t B1 = B / g1;
        seq_t denom1 = denom / g1;

        seq_t g2 = gcd(D, denom1);
        seq_t D1 = D / g2;
        denom1 = denom1 / g2;  /* should be 1 now */

        seq_t term = ((seq_t)(k + 1)) * B1 * D1;
        sum += term;

        /* update B : C(2n-(k+1), n) = B * (n-k) / (2n-k) */
        B = (B * (seq_t)(n - k)) / denom;

        /* update D : C(3n-(k+1), 2n+1) = D * (n-k-1) / (3n-k) */
        D = (D * (seq_t)(n - k - 1)) / (seq_t)(three_n - k);
    }

    return sum;
}
