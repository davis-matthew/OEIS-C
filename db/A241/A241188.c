/*
 * A241188  Triangle T(n,s) of Dynkin type D_n read by rows (n >= 2, 0 <= s <=
 *          n).
 * https://oeis.org/A241188
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by flattening the triangle T(n,s) of Dynkin
 *            type D_n. Each entry is T(n,s) = binomial(n+s-2, s) * (n+2s-2) /
 *            (n+s-2) for 0 <= s < n T(n,n) = binomial(2n-2, n-2) * (3n-4) /
 *            (2n-2)
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* cumulative number of entries up to row r (inclusive) */
static inline idx_t cumulative(idx_t r)
{
    if (r < 2) return 0;
    return (idx_t)r * (r + 1) / 2 + r - 2;
}

/* Return the n-th term of the sequence, where a(2) is the first term. */
seq_t a(idx_t n)
{
    /* Sequence undefined for n < 2; return 0 for safety. */
    if (n < 2) return 0;

    idx_t m = n - 2;            /* zero‑based linear index */

    /* Find the row r such that cumulative(r-1) <= m < cumulative(r). */
    idx_t low = 2, high = 2;
    while (cumulative(high) <= m) {
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cumulative(mid) > m)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* first row where cumulative(r) > m */
    idx_t prev = cumulative(r - 1);    /* entries before row r */
    idx_t s = m - prev;                /* position within row r */

    seq_t val;
    if (s == r) { /* special last entry of the row */
        idx_t t = 2 * r - 2;
        idx_t s_special = r - 2;
        val = binomial(t, s_special);
        val = val * ((seq_t)3 * r - 4);
        val = val / (seq_t)t;
    } else { /* normal entries */
        idx_t t = r + s - 2;
        if (t == 0) return 1;           /* only occurs for n=2, s=0 */
        val = binomial(t, s);
        val = val * (t + s);
        val = val / (seq_t)t;
    }
    return val;
}
