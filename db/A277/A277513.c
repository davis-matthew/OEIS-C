/*
 * A277513  Irregular triangle read by rows: T(n,k) is the number of integers
 *          greater than 4 such that they have n trits and 2k+1 (k>=1) nonzero
 *          trits in their balanced ternary representation, with n>=3 and
 *          1<=k<=(j-1)/2.
 * https://oeis.org/A277513
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, nonn, tabf
 * Formula:   a(n) = 4^k * binomial(r‑1, 2k), where (r,k) are the * unranked
 *            pair (r ≥ 3, 1 ≤ k ≤ floor((r‑1)/2)) corresponding to * index
 *            n–2 in the irregular triangle. Offset: a(3) is the first *
 *            listed term
 * Range:     u8 n <= 12, u16 n <= 35, u32 n <= 118, u64 n <= 455,
 *            u128 n <= 1709, big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/pow_int.h"    /* pow_int(seq_t base, idx_t exp) */

/* cumulative number of terms up to and including row r (r ≥ 3) */
static inline idx_t cumulative_terms(idx_t r)
{
    if (r < 3) return 0;
    if (r & 1) {                 /* r odd */
        idx_t t = (r - 1) >> 1;   /* (r‑1)/2 */
        return t * t;             /* t² */
    } else {                      /* r even */
        idx_t t = (r - 2) >> 1;   /* (r‑2)/2 */
        return t * (t + 1);       /* t·(t+1) */
    }
}

seq_t a(idx_t n)
{
    /* offset 3: a(3) is the first term */
    if (n < 3) return 0;           /* undefined for n < 3 */
    idx_t idx = n - 2;             /* 1‑based position in the triangle */

    /* find the row r where idx lies */
    idx_t r = 3;
    while (cumulative_terms(r) < idx) ++r;

    idx_t prev = cumulative_terms(r - 1);
    idx_t pos  = idx - prev - 1;    /* 0‑based position within the row */
    idx_t k     = pos + 1;          /* k starts at 1 */

    idx_t m = r - 1;                /* choose from m = r‑1 */
    idx_t k2 = 2 * k;               /* 2k */
    seq_t bin = binomial(m, k2);    /* C(m, 2k) */
    seq_t pow4k = pow_int((seq_t)4, k);  /* 4^k = 2^(2k) */
    return bin * pow4k;
}
