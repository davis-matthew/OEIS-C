/*
 * A386235  Number of partitions (p, q, r) of n into positive integers such
 *          that p + 11*q + 13*r is a perfect square.
 * https://oeis.org/A386235
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 197, u16 n <= 8006, u32 n <= 8168, u64 n <= 8168,
 *            u128 n <= 8168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of positive integer triples (p,q,r) with
 * p+q+r = n such that p + 11*q + 13*r is a perfect square.
 * The function implements OEIS sequence A386235.
 *
 * The algorithm iterates over all square values s with
 *      n + 10*q + 12*r = s²
 * and solves the linear Diophantine equation
 *      5*q + 6*r = (s² - n)/2
 * for positive integers q,r that also satisfy q + r ≤ n‑1.
 * This yields all admissible triples (p,q,r) with p = n-q-r.  */
#include <stdint.h>

/* Integer square‑root for 64‑bit unsigned numbers.
 * Returns ⌊√n⌋.  Uses a simple binary search. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Offset is 3; results for n < 3 are irrelevant. */
    if (n < 3) return (seq_t)0;

    /* s² must satisfy n + 10·1 + 12·1 ≤ s² ≤ n + 10·1 + 12·(n‑2) */
    idx_t min_s = isqrt_u64(n + 22);
    if (min_s * min_s < n + 22) ++min_s;

    idx_t max_s = isqrt_u64(13 * n - 14);
    if (max_s * max_s > 13 * n - 14) --max_s;

    seq_t count = 0;

    for (idx_t s = min_s; s <= max_s; ++s) {
        idx_t ss = s * s;
        idx_t t  = ss - n;              /* t = 10*q + 12*r */
        if (t & 1) continue;            /* t must be even */

        idx_t tp = t >> 1;              /* tp = 5*q + 6*r */

        /* Compute admissible range for r.
         *  r ≥ 1
         *  q > 0  ⇒  tp - 6r ≥ 5  ⇒  r ≤ (tp - 5)/6
         *  q + r ≤ n-1  ⇒  r ≥ tp - 5*(n-1)
         *  r ≤ n-2
         */
        idx_t lower = 1;
        idx_t temp = tp > 5 * (n - 1) ? tp - 5 * (n - 1) : 1;
        if (temp > lower) lower = temp;

        idx_t upper = (tp - 5) / 6;
        if (upper > n - 2) upper = n - 2;
        if (upper < lower) continue;

        /* r must satisfy r ≡ tp (mod 5) because 6r ≡ r (mod 5). */
        idx_t desired = tp % 5;
        idx_t r_start = lower + (desired - (lower % 5) + 5) % 5;
        if (r_start > upper) continue;

        for (idx_t r = r_start; r <= upper; r += 5) {
            /* q = (tp - 6r) / 5 is guaranteed integer by construction. */
            idx_t q = (tp - 6 * r) / 5;
            if (q < 1) continue;               /* safety check */
            if (q + r > n - 1) continue;        /* safety check */
            /* p = n - q - r is automatically positive. */
            ++count;
        }
    }

    return count;
}
