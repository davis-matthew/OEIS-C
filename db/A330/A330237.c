/*
 * A330237  Square array T(n,k): concatenate the absolute differences of the
 *          digits of n and k (the smaller one padded with leading zeros);
 *          read by antidiagonals; n, k >= 1.
 * https://oeis.org/A330237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A330237 – Square array T(n,k) read by antidiagonals
 *  
 *  The sequence is defined by the symmetric table
 *      T(i,j) = concat(|d_t(i) – d_t(j)|) for all i,j ≥ 1,
 *  where d_t(x) are the decimal digits of x padded with leading zeros to
 *  the length of the longer number.  The sequence a(n) listed in OEIS
 *  is obtained by reading the table along its antidiagonals:
 *
 *      s = 2,3,4,…  (sum i+j)
 *      for each s, i runs from 1 to s‑1 and j = s‑i
 *
 *  Hence the n‑th term corresponds to the pair (i,j) where
 *
 *      s = minimal integer with (s‑1)*s/2 ≥ n
 *      i = n – (s‑2)*(s‑1)/2
 *      j = s – i
 *
 *  The function below implements this definition.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: compute T(i,j) – concatenation of absolute digit differences. */
static inline seq_t T(idx_t i, idx_t j)
{
    idx_t a = i, b = j;
    int digs[64];          /* maximum number of decimal digits for 64‑bit values */
    int k = 0;

    /* extract digits from least significant to most significant,
       computing the absolute differences */
    while (a > 0 || b > 0) {
        int da = a % 10; a /= 10;
        int db = b % 10; b /= 10;
        int d  = (da > db) ? (da - db) : (db - da);
        digs[k++] = d;
    }

    /* build the concatenated number from most significant to least */
    seq_t res = 0;
    for (int p = k - 1; p >= 0; --p)
        res = res * 10 + (seq_t)digs[p];

    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that the nth term lies on it. */
    idx_t s = 2;
    __uint128_t cum = 1;            /* (s‑1)*s/2 for current s */
    while (cum < n) {
        ++s;
        cum += (__uint128_t)(s - 1);
    }

    /* Off‑by‑one correction: count of pairs before this diagonal */
    idx_t prev_cum   = cum - (s - 1);          /* (s‑2)*(s‑1)/2 */
    idx_t offset     = n - prev_cum;           /* 1‑based position on the diagonal */
    idx_t i = offset;
    idx_t j = s - i;

    return T(i, j);
}
