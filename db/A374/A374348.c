/*
 * A374348  a(n) = k where wt(k) = n and k + wt(k) = a power of two, where
 *          wt(n) = A000120(n) = binary weight of n.
 * https://oeis.org/A374348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(n + A000120(n-1)) - n
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2^(n + popcount(n-1)) - n, where popcount is the binary weight of n-1 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static inline int popcount64(uint64_t x) {
    #if defined(__GNUC__) || defined(__clang__)
        return __builtin_popcountll(x);
    #else
        int c = 0;
        while (x) { c += x & 1ull; x >>= 1; }
        return c;
    #endif
}

seq_t a(idx_t n) {
    /* n >= 1 by definition of the sequence */
    uint64_t n_minus1 = n - 1ULL;
    int w = popcount64(n_minus1);
    idx_t exponent = n + (idx_t)w;
    seq_t value = pow2(exponent);   /* 2^exponent */
    value -= (seq_t)n;              /* subtract n, under unsigned arithmetic */
    return value;
}
