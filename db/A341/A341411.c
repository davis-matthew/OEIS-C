/*
 * A341411  a(n) = (Sum_{k=1..5} k^n) mod n.
 * https://oeis.org/A341411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1^n + 2^n + 3^n + 4^n + 5^n) mod n
 * Range:     u8 n <= 264, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1; n>=1 in normal use */
    seq_t sum = 0;
    for (idx_t k = 1; k <= 5; ++k) {
        seq_t term = mod_pow((seq_t)k, n, (seq_t)n);
        sum += term;
    }
    return sum % (seq_t)n;
}
