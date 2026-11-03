/*
 * A264902  Number T(n,k) of defective parking functions of length n and
 *          defect k; triangle T(n,k), n>=0, 0<=k<=max(0,n-1), read by rows.
 * https://oeis.org/A264902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the flattened T(n,k) where T(r,k) = S(r,k) - S(r,k+1)
 *            with S(r,0) = r^r S(r,k) = Σ_{i=0..r-k} C(r,i) * k * (k+i)^(i-1)
 *            * (r-k-i)^(r-i) for k>0 The flattening is by rows: row r has
 *            max(1,r) entries
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Validated: standard, pass, 62 terms through n = 8861 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Helper: compute S(r,k) for 0 ≤ k ≤ r; returns 0 when k>r. */
static seq_t S(idx_t r, idx_t k)
{
    if (k > r)
        return 0;                            /* empty sum */
    if (k == 0)
        return ipow_idx((seq_t)r, r);          /* r^r, works for r=0 as 0^0 = 1 */

    seq_t sum = 0;
    for (idx_t i = 0; i <= r - k; ++i) {
        seq_t term;
        if (i == 0) {
            /* (r-k)^r */
            term = ipow_idx((seq_t)(r - k), r);
        } else {
            term = binomial(r, i);                          /* C(r,i) */
            term = seq_mul(term, (seq_t)k);                 /* *k      */
            term = seq_mul(term,                   /* *(k+i)^(i-1) */
                           ipow_idx((seq_t)(k + i), i - 1));
            term = seq_mul(term,                  /* * (r-k-i)^(r-i) */
                           ipow_idx((seq_t)(r - k - i), r - i));
        }
        sum += term;
    }
    return sum;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    /* Find row r and column k from flattened index n. */
    idx_t r = 0, offset = n;
    while (true) {
        idx_t len = (r == 0) ? 1 : r; /* length of current row */
        if (offset < len)
            break;                    /* found the row */
        offset -= len;
        ++r;
    }
    idx_t k = offset;                 /* column within the row */

    seq_t s_k  = S(r, k);
    seq_t s_k1 = S(r, k + 1);
    return s_k - s_k1;
}
