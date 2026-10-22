/*
 * A333445  Table T(n,k) read by upward antidiagonals. T(n,k) =
 *          Product_{i=1..n} Sum_{j=1..k} (i-1)*k+j.
 * https://oeis.org/A333445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) where T(i,k)=∏_{m=1}^{i} Σ_{j=1}^{k} ( (m-1)*k + j
 *            ), read along upward antidiagonals (i decreases, k increases)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 38, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute s = ceil((1 + sqrt(1 + 8*n))/2) using integer sqrt. */
    idx_t D = 1 + 8 * n;

    /* Binary search integer sqrt of D. */
    idx_t l = 0, r = D;
    while (l <= r) {
        idx_t m = l + ((r - l) >> 1);
        unsigned __int128 sq = (unsigned __int128)m * m;
        if (sq <= D) {
            l = m + 1;
        } else {
            if (m == 0) break;          /* safety guard, D>0 so not taken */
            r = m - 1;
        }
    }
    idx_t sqrtD = r;          /* floor sqrt(D) */
    if ((unsigned __int128)sqrtD * sqrtD < D) sqrtD++;   /* ceil sqrt(D) if needed */
    idx_t s = (sqrtD + 2) / 2;                           /* ceil((1+sqrtD)/2) */

    /* Coordinates on the antidiagonal. */
    idx_t prior = (s - 1) * (s - 2) / 2;   /* terms before this diagonal */
    idx_t offset = n - prior - 1;           /* 0‑based position within diagonal */
    idx_t i = s - 1 - offset;              /* row index */
    idx_t k = offset + 1;                  /* column index */

    /* Compute T(i,k). */
    seq_t result = 1;
    seq_t k_seq = (seq_t)k;
    seq_t k_sq   = k_seq * k_seq;
    seq_t sum_k  = k_seq * ((seq_t)(k + 1)) / 2;

    for (idx_t m = 1; m <= i; ++m) {
        seq_t part = k_sq * ((seq_t)(m - 1)) + sum_k;
        result *= part;
    }

    return result;
}
