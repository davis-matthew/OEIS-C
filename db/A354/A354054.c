/*
 * A354054  a(n) = Product_{k=0..n} (k^6 + 1).
 * https://oeis.org/A354054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^6 + 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 103 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t product_range(idx_t lo, idx_t hi)
{
    if (lo > hi)
        return (seq_t)1;                      /* empty product */
    if (lo == hi) {
        seq_t k = (seq_t)lo;
        seq_t k3 = k * k * k;                /* k^3 */
        seq_t k6 = k3 * k3;                  /* k^6 */
        return k6 + (seq_t)1;                /* k^6 + 1 */
    }
    idx_t mid = lo + ((hi - lo) >> 1);       /* avoid overflow */
    return product_range(lo, mid) * product_range(mid + 1, hi);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1)  /* just in case of unsigned wrap-around */
        return (seq_t)1;
    return product_range((idx_t)0, n);
}
