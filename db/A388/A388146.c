/*
 * A388146  Array read by antidiagonals: T(n,k) is the number of length n
 *          binary words in which 1's occur in blocks of at least k, n >= 0, k
 *          >= 1.
 * https://oeis.org/A388146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{i=0}^{⌊(m+1)/(k+1)⌋} binomial(m+1-(k-1)i, 2i) where
 *            (m,k) is the antidiagonal pair determined by s =
 *            ⌊(√(8n+1)+1)/2⌋, d = n - (s-1)s/2, m = d, k = s - d (s≥1, d
 *            ranges from 0 to s-1). The binomial coefficients are computed
 *            exactly by the helper function binomial(a,b), returning a
 *            seq_t...
 * Range:     u8 n <= 34, u16 n <= 151, u32 n <= 559, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static idx_t int_sqrt(idx_t v)
{
    /* Integer square root: floor(sqrt(v)) */
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* avoid overflow: check mid > v/mid instead of mid*mid > v */
        if (mid != 0 && mid > v / mid) {
            high = mid - 1;
        } else {
            if ((mid + 1) * (mid + 1) > v) {
                return mid;
            }
            low = mid + 1;
        }
    }
    return 0; /* unreachable for v≥0 */
}

seq_t a(idx_t n)
{
    /* 1. Find the diagonal (sum s) containing index n */
    idx_t t      = 8 * n + 1;                 /* 8n+1  fits in idx_t */
    idx_t root   = int_sqrt(t);               /* floor sqrt(8n+1) */
    idx_t s      = (root + 1) / 2;            /* s = floor((√(8n+1)+1)/2) */
    /* 2. Find offset inside this diagonal */
    idx_t prev_cnt = (s - 1) * s / 2;         /* number of terms before diagonal s */
    idx_t d        = n - prev_cnt;            /* offset: 0 .. s-1 */
    idx_t m        = d;                       /* length of words */
    idx_t k        = s - d;                   /* required block size */

    /* 3. Compute T(m,k) by its summation formula */
    idx_t i_max = (m + 1) / (k + 1);          /* floor((m+1)/(k+1)) */
    seq_t sum = 0;
    for (idx_t i = 0; i <= i_max; ++i) {
        idx_t a_val = m + 1 - (k - 1) * i;    /* numerator of binomial */
        idx_t b_val = 2 * i;                  /* denominator of binomial */
        seq_t term  = binomial(a_val, b_val);
        sum += term;
    }
    return sum;
}
