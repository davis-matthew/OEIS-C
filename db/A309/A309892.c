/*
 * A309892  a(0) = 0, a(1) = 1, and for any n > 1, a(n) is the number of
 *          iterations of the map x -> x - gpf(x) (where gpf(x) denotes the
 *          greatest prime factor of x) required to reach 0 starting from n.
 * https://oeis.org/A309892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> x - gpf(x) to reach 0
 *            starting from n. a(0) = 0, a(1) = 1
 * Range:     u8 n <= 9968, u16 n <= 9968, u32 n <= 9968, u64 n <= 9968,
 *            u128 n <= 9968, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)n;            /* a(0)=0, a(1)=1 */
    }

    seq_t cnt = 0;
    uint64_t x = (uint64_t)n;

    while (x > 0) {
        uint64_t p = gpf(x);       /* greatest prime factor of x */
        x -= p;
        cnt++;
    }

    return cnt;
}
