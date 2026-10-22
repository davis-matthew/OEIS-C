/*
 * A359710  Order of shifts of Thue-Morse sequence.
 * https://oeis.org/A359710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th element of the lexicographic order of shifts of
 *            the Thue‑Morse sequence. * For a given n, let k =
 *            floor(log2(n+1)). The first element of group k starts at
 *            position 2^k-1. * Let r = n - (2^k-1). Then a(n) = g_k(r), where
 *            g_1(1)=0, g_1(2)=1 and for k>1: * g_k(2i) = 2^{k-1} + g_{k-1...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_elem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_elem.h"

static inline unsigned long long floor_log2_idx(unsigned long long x) {
    /* x >= 1 */
    return 63 - __builtin_clzll(x);
}

seq_t a(idx_t n) {
    idx_t x = n + 1;
    idx_t k = floor_log2_idx(x);                  /* group index */
    idx_t start = (((idx_t)1) << k) - 1;           /* first position of this group */
    idx_t r = n - start + 1;                      /* position within the group, 1‑based */
    return get_elem(k, r);
}
