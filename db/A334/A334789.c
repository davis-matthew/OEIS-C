/*
 * A334789  a(n) = 2^log_2*(n) where log_2*(n) = A001069(n) is the number of
 *          log_2(log_2(...log_2(n))) iterations needed to reach < 2.
 * https://oeis.org/A334789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^A001069(n) (iterated binary log)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned floor_log2_uint64(idx_t v) {
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    if (n <= 1)
        return 1;
    unsigned count = 0;
    idx_t v = n;
    while (v > 1) {
        v = (idx_t)floor_log2_uint64(v);
        ++count;
    }
    seq_t res = 1;
    res <<= count;   /* multiply by 2^count */
    return res;
}
