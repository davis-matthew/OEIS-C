/*
 * A329717  a(n) is n (plus or minus) the number of distinct primes dividing n
 *          according to parity (even or odd).
 * https://oeis.org/A329717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A001221(n)*(-1)^A001221(n)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9661 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    seq_t k = (seq_t)omega((uint64_t)n);
    seq_t nn = (seq_t)n;
    if ((k & 1) == 0) {
        return nn + k;
    } else {
        return nn - k;
    }
}
