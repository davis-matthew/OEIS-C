/*
 * A246685  Run Length Transform of sequence 1, 3, 5, 17, 257, 65537, ... (1
 *          followed by Fermat numbers).
 * https://oeis.org/A246685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of b(run_length_i) * where run_length_i are the
 *            lengths of successive 1-runs in the binary * expansion of n. *
 *            b(1) = 1 * b(k) = 2^(2^(k-2)) + 1 for k >= 2 (Fermat numbers)
 * Range:     u8 n <= 30, u16 n <= 62, u32 n <= 126, u64 n <= 254,
 *            u128 n <= 510, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t fermat_value(idx_t len) {
    if (len <= 1) return 1;          /* b(1) = 1 */
    /* exp = 2^(len-2) */
    idx_t exp = (idx_t)1 << (len - 2);
    return pow2(exp) + 1;            /* 2^exp + 1 */
}

seq_t a(idx_t n) {
    seq_t prod = 1;
    idx_t run = 0;
    while (n != 0) {
        if (n & 1) {
            /* bit is 1, extend current run */
            run++;
        } else {
            /* bit is 0, end of a run (if any) */
            if (run) {
                prod *= fermat_value(run);
                run = 0;
            }
        }
        n >>= 1;
    }
    /* handle a run that ends at the most significant bit */
    if (run) {
        prod *= fermat_value(run);
    }
    return prod;
}
