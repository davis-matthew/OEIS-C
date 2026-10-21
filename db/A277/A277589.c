/*
 * A277589  Numbers k such that k/10^m == 2 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose rightmost non‑zero digit is 2 (OEIS A277589) */
#include <stdint.h>
#include "../utils/count_upto.h"

/* Return the n-th term (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the offset below 1 */
    seq_t low  = 1;
    seq_t high = (seq_t)n * (seq_t)10;   /* rough upper bound, density ≈ 1/9 */

    /* Expand the search range until it covers at least n terms. */
    while (count_upto(high) < (seq_t)n) {
        high *= 2;
    }

    /* Binary search for the smallest X with count >= n. */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        if (count_upto(mid) >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}
