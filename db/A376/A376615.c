/*
 * A376615  a(n) is the number of iterations that n requires to reach a
 *          noninteger under the map x -> x / wt(x), where wt(k) is the binary
 *          weight of k (A000120); a(n) = 0 if n is a power of 2.
 * https://oeis.org/A376615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of iterations of the map x -> x / wt(x) required
 *            to reach a non‑integer. a(n) = 0 if n is a power of two (wt(2^k)
 *            = 1)
 * Range:     u8 n <= 5951, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"

static inline int popcount_seq_t(seq_t x) {
    int bits = 0;
    while (x) {
        x &= x - 1;
        bits++;
    }
    return bits;
}

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;

    /* Powers of two are fixed points, so they map to 0. */
    if (is_power_of_two(cur))
        return 0;

    seq_t count = 0;
    for (;;) {
        int w = popcount_seq_t(cur);          /* wt(cur) */
        if (cur % (seq_t)w == 0) {            /* next value is an integer */
            cur = cur / (seq_t)w;
            count++;
        } else {                              /* next value is a non‑integer */
            count++;
            break;
        }
    }
    return count;
}
