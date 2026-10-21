/*
 * A277573  a(n) = (1/3)*A277569(n).
 * https://oeis.org/A277573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n-th integer k (k ≥ 1) such that v₃(k) ≥ v₂(k),
   i.e. the 3-adic valuation is not smaller than the 2-adic valuation.
   The function increments k and counts qualifying values until
   the n-th one is found. */

#include <stdbool.h>

/* Return true iff the integer n satisfies v₃(n) ≥ v₂(n). */
static inline bool good(seq_t n) {
    idx_t v2 = 0, v3 = 0;
    seq_t tmp;

    /* compute v₂(n) */
    tmp = n;
    while ((tmp & (seq_t)1) == 0) {
        v2++;
        tmp >>= 1;
    }

    /* compute v₃(n) */
    tmp = n;
    while (tmp % (seq_t)3 == 0) {
        v3++;
        tmp /= 3;
    }

    return v3 >= v2;
}

seq_t a(idx_t n) {
    seq_t x = 1;
    idx_t count = 0;

    while (1) {
        if (good(x)) {
            count++;
            if (count == n)
                return x;
        }
        x++;
    }
}
