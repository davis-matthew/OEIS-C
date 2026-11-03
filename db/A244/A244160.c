/*
 * A244160  a(0)=0, and for n >= 1, a(n) = the largest k such that k-th
 *          Catalan number <= n.
 * https://oeis.org/A244160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that Catalan(k) <= n
 * Range:     u8 n <= 21, u16 n <= 2490, u32 n <= 4861, u64 n <= 4861,
 *            u128 n <= 4861, big any n
 * Validated: standard, pass, 149 terms through n = 4613 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses binary search with catalan(idx_t) from utils */
#include <stddef.h>
#include <stdlib.h>

/* multiplicative computation of the k‑th Catalan number */
static seq_t catalan(idx_t k)
{
    seq_t c = (seq_t)1;          /* C(0) = 1 */
    for (idx_t i = 1; i <= k; ++i) {
        /* C(i) = C(i-1) * (4*i - 2) / (i + 1) */
        seq_t num = c * (seq_t)(4 * i - 2);
        seq_t den = (seq_t)(i + 1);
        c = num / den;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0) return 0;

    /* Search bounds for k */
    idx_t low  = 1;
    idx_t high = 1;

    /* Grow the upper bound until Catalan(high) > n */
    while (catalan(high) <= (seq_t)n) {
        /* Avoid overflow: if high already near max, break */
        if (high > ((idx_t)(-1)) / 2) {
            high = (idx_t)(-1);
            break;
        }
        high <<= 1;
    }

    /* Binary search the largest k with Catalan(k) <= n */
    idx_t best = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t cat = catalan(mid);
        if (cat <= (seq_t)n) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* safeguard against underflow */
            high = mid - 1;
        }
    }

    return (seq_t)best;
}
