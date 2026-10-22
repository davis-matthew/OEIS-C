/*
 * A385458  Triangle read by rows: T(n,k) = exponent of the highest power of 2
 *          dividing each Fibonomial coefficient fibonomial(n, k).
 * https://oeis.org/A385458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where row,col are derived from the flattened
 *            index n. T(row,col) = b(row) - b(col) - b(row-col). And b(m) =
 *            2*floor(m/3) + floor(m/6) - popcount(floor(m/3)), the 2-adic
 *            valuation of the product of the first m Fibonacci numbers
 * Range:     u8 n <= 9470, u16 n <= 9470, u32 n <= 9470, u64 n <= 9470,
 *            u128 n <= 9470, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper to compute b(m).  m is non-negative and fits in idx_t. */
static inline seq_t b_value(idx_t m) {
    idx_t m3 = m / 3U;
    idx_t m6 = m / 6U;
    /* popcount of m3 */
    unsigned long long pop = __builtin_popcountll((unsigned long long)m3);
    return (seq_t)( (seq_t)2U * (seq_t)m3 + (seq_t)m6 - (seq_t)pop );
}

seq_t a(idx_t n) {
    /* Find row 'r' such that cumulative rows up to r-1 <= n < cumulative up to r. */
    idx_t r = 0U;
    idx_t cum = 0U;          /* total entries up to row r-1 */
    while (cum + (r + 1U) <= n) {
        cum += r + 1U;
        r++;
    }

    /* Column index within the row. */
    idx_t c = n - cum;       /* 0 <= c <= r */

    /* Compute the exponent of 2 dividing the fibonomial coefficient. */
    seq_t br = b_value(r);
    seq_t bc = b_value(c);
    seq_t brc = b_value(r - c);
    return br - bc - brc;
}
