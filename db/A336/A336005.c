/*
 * A336005  a(n) is the number of terms in the mixed binary-ternary
 *          representation of n. See Comments.
 * https://oeis.org/A336005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of terms in the greedy mixed binary‑ternary
 *            representation of n. The multiset of allowed summands is {2^i,
 *            3^j, 2·3^k | i,j,k ≥ 0, 1}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_pow2, largest_pow3, largest_two_times_pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_pow2.h"
#include "../utils/largest_pow3.h"
#include "../utils/largest_two_times_pow3.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    while (n > 0) {
        idx_t pow2  = largest_pow2(n);
        idx_t pow3  = largest_pow3(n);
        idx_t pow23 = largest_two_times_pow3(n);

        idx_t best = pow2;
        if (pow3 > best) best = pow3;
        if (pow23 > best) best = pow23;

        n -= best;
        ++count;
    }
    return (seq_t)count;
}
