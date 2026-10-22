/*
 * A337496  Number of bases b for which the expansion of n in base b contains
 *          the largest digit possible (i.e., the digit b-1).
 * https://oeis.org/A337496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of bases b>1 such that the base‑b expansion of n
 *            contains the digit b-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for all n ≥ 0.  For n=0 the answer is 0.  For n>0, base 2 and base n+1 are
   always counted; all other bases are tested by iterating over the digits of n in that base. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t count = 0;
    /* limit = n+1, but avoid overflow when n==UINT64_MAX */
    idx_t limit = (n == UINT64_MAX) ? n : n + 1;

    for (idx_t b = 2; b <= limit; ++b) {
        idx_t tmp = n;
        /* Check each digit of n in base b.
           If any digit equals b-1, this base is a main base. */
        while (tmp) {
            if (tmp % b == b - 1) {
                ++count;
                break;
            }
            tmp /= b;
        }
    }
    return count;
}
