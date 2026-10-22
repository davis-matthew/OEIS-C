/*
 * A384125  Array read by antidiagonals: T(n,m) is the number of edges in the
 *          n X m rook graph K_n X K_m.
 * https://oeis.org/A384125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where (i,j) are chosen by antidiagonal traversal *
 *            of the infinite array T(n,m) = n*binomial(m,2)+m*binomial(n,2).
 *            * * The sequence is defined by enumerating the pairs (i,j) with
 *            i,j>=1 * in increasing order of s=i+j, and within each diagonal
 *            listing * them in order of increasing i...
 * Range:     u8 n <= 82, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t cumulative(idx_t s) {
    /* cumulative count of terms up to and including diagonal sum s:
     * sum_{t=2}^{s} (t-1) = (s-1)*s/2
     */
    unsigned __int128 tmp = (unsigned __int128)(s-1) * s;
    return (idx_t)(tmp / 2);
}

static idx_t find_s(idx_t n) {
    /* Find smallest s (>=2) such that cumulative(s) >= n. */
    if (n == 0) return 0;
    idx_t low = 2, high = 2;
    while (cumulative(high) < n) {
        high <<= 1;
        if (high < low) { /* overflow guard, n is too large */
            high = (idx_t)-1;
            break;
        }
    }
    /* binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cumulative(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static inline seq_t binom2(idx_t k) {
    if (k < 2) return 0;
    return ((seq_t)k * (seq_t)(k-1)) / 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but safe */

    idx_t s = find_s(n);                     /* diagonal sum i+j = s */
    idx_t prev_cum = cumulative(s - 1);      /* count before this diagonal */
    idx_t offset = n - prev_cum;              /* 1-based offset within diag */

    idx_t i = offset;                        /* i runs 1..s-1 */
    idx_t j = s - i;                          /* j = s-i */

    seq_t term = (seq_t)i * binom2(j) + (seq_t)j * binom2(i);
    return term;
}
