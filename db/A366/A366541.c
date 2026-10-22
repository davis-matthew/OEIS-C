/*
 * A366541  Regular triangle read by rows: T(n,k) is the number of k-facets of
 *          the bipyramid on an n-simplex base.
 * https://oeis.org/A366541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the row‑major read of the triangle T(n,k) = 2*C(n+2,k+1)
 *            – C(n+1,k+1) (0 ≤ k < n) T(n,n) = 2*(n+1) where C(a,b) is the
 *            binomial coefficient. a(0) = 2, a(1) = 4…
 * Range:     u8 n <= 38, u16 n <= 143, u32 n <= 574, u64 n <= 2175,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial(n,k) for unsigned integer types */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            /* 128‑bit intermediate keeps the product safe on 64‑bit builds. */
            unsigned __int128 prod = (unsigned __int128)res * (n - k + i);
            res = (seq_t)(prod / i);
        } else {
            /* For arbitrary‑precision seq_t assume operator* and / work. */
            seq_t num   = (seq_t)(n - k + i);
            seq_t denom = (seq_t)i;
            res = (res * num) / denom;
        }
    }
    return res;
}

/*  Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = n, best = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t cum = mid * (mid + 1) / 2;
        if (cum <= n) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    idx_t r   = find_row(n);          /* row number          */
    idx_t cum = r * (r + 1) / 2;      /* elements before row*/
    idx_t k   = n - cum;              /* column within row  */

    /* Special case of the single element row */
    if (r == 0) return (seq_t)2;

    /* Last element of a row */
    if (k == r) return (seq_t)2 * ((seq_t)r + 1);

    /*  T(r,k) = 2*C(r+1,k) + C(r+1,k+1)  */
    seq_t C = binom_small(r + 1, k);
    /* C(r+1,k+1) = C(r+1,k) * (r+1-k) / (k+1) */
    seq_t Cnext;
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 prod = (unsigned __int128)C * (r + 1 - k);
        Cnext = (seq_t)(prod / (k + 1));
    } else {
        seq_t num   = (seq_t)(r + 1 - k);
        seq_t denom = (seq_t)(k + 1);
        Cnext = (C * num) / denom;
    }

    return (seq_t)((C << 1) + Cnext);
}
