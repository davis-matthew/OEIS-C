/*
 * A338035  Triangle T(n,m) = (1/m)*Sum_{k=1..m}
 *          k*C(2*m-k-1,m-k)*C(2*(2*m-k),n-2*m+k), n>0, m>0.
 * https://oeis.org/A338035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A338035: triangle T(n,m) = (1/m)*∑_{k=1..m} k*C(2m-k-1,m-k)*C(2*(2m-k),n-2m+k),
   enumerated row‑wise: a(n)=T(r,c) where r(r+1)/2≥n and c=n−r(r−1)/2. */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Determine the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) ++r;
    /* Position c within that row (1‑based) */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;          /* 1 ≤ c ≤ r */

    /* Compute T(r,c) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= c; ++k) {
        /* First binomial component */
        idx_t a1 = 2 * c - k - 1;
        idx_t b1 = c - k;
        if (a1 < b1) continue;   /* binomial would be zero */

        seq_t term1 = binomial(a1, b1);

        /* Second binomial component */
        idx_t a2 = 2 * (2 * c - k);
        /* Compute index inside second binomial safely */
        int64_t diff = (int64_t)r - (int64_t)2 * (int64_t)c + (int64_t)k;
        if (diff < 0 || diff > (int64_t)a2) continue;  /* binomial would be zero */

        seq_t term2 = binomial(a2, (idx_t)diff);

        seq_t mul = term1 * term2;
        mul *= (seq_t)k;
        sum += mul;
    }
    /* Division by m (=c) is exact */
    return sum / c;
}
