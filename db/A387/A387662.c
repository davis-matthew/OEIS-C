/*
 * A387662  a(n) is the least k>0 such that n divides 1^1 + 2^2 + ... + k^k.
 * https://oeis.org/A387662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k>0 such that n divides 1^1 + 2^2 + ... + k^k
 * Range:     u8 n <= 85, u16 n <= 8156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    uint64_t sn_u = n;
    uint64_t sum_mod = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t term_seq = mod_pow((seq_t)k, (uint64_t)k, sn);
        uint64_t term = (uint64_t)term_seq;
        sum_mod += term;
        if (sum_mod >= sn_u) sum_mod %= sn_u;
        if (sum_mod == 0) return (seq_t)k;
    }
}
