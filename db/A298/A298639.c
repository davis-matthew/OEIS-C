/*
 * A298639  Numbers k such that the digital sum of k and the digital root of k
 *          have the same parity.
 * https://oeis.org/A298639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute a(n) for OEIS A298639.
    Definition: a(n) is the n-th non‑negative integer k (with a(1)=0)
    for which the parity of the decimal digit sum of k equals the
    parity of the decimal digital root of k.
    This is implemented by a simple linear scan – for each integer
    k the function checks the parity condition and counts accepted
    values until the n-th one is found.  The algorithm is O(n). */
/*  Requires the helper function:
        seq_t digit_sum(seq_t v);
    which is defined in "../utils/digit_sum.h". */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/*  Return true iff the parity of the digit sum of k agrees with
    the parity of its digital root.  Digital root parity is 0 for 0,
    1 if k % 9 == 0 (root 9), otherwise the parity of k % 9. */
static bool parity_condition(seq_t k) {
    seq_t sum   = digit_sum(k);
    bool parity_sum = (sum & 1) != 0;

    bool parity_root;
    if (k == (seq_t)0) {
        parity_root = false;             /* 0 is even */
    } else {
        seq_t mod9 = k % 9;
        if (mod9 == (seq_t)0) {
            parity_root = true;          /* root = 9, odd */
        } else {
            parity_root = ((mod9 & 1) != 0);
        }
    }
    return parity_sum == parity_root;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;

    /*  scan until the n‑th acceptable number is found  */
    while (true) {
        if (parity_condition(k)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
