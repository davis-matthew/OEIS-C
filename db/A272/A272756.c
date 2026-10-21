/*
 * A272756  a(n) is the least k such that k > A070939(n * k).
 * https://oeis.org/A272756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 535, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272756:  a(n) = least k > A070939(n*k), where A070939(m)
   = floor(log2(m)) + 1 (the binary digit length of m).  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen128.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    for (seq_t k = 1; ; ++k) {
        unsigned __int128 prod = (unsigned __int128)N * (unsigned __int128)k;
        unsigned bits = bitlen128(prod);   /* floor(log2(prod)) + 1 */
        if (k > bits) return k;
    }
}
