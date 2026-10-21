/*
 * A272759  a(n) = A065879(n)/n.
 * https://oeis.org/A272759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): least i ≥ 1 such that popcount(n*i) == i; returns 0 if none. */

#include <stdbool.h>
#include <stdint.h>

static unsigned popcount_uint64(uint64_t x) {
    return (unsigned)__builtin_popcountll(x);
}

static unsigned popcount_uint128(unsigned __int128 x) {
    return popcount_uint64((uint64_t)x) + popcount_uint64((uint64_t)(x >> 64));
}

static unsigned bitlen_uint64(uint64_t x) {
    return x == 0 ? 0 : 64 - __builtin_clzll(x);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* offset is 1, safety guard */

    /* Conservative upper bound for i.  For all tested n this is ample. */
    unsigned bitlen_n = bitlen_uint64((uint64_t)n);
    unsigned max_i = bitlen_n + 20;          /* 20 enough for all n ≤ 10 000 */
    if (max_i < 200) max_i = 200;            /* also allow a fixed margin */

    for (unsigned i = 1; i <= max_i; ++i) {
        unsigned __int128 prod = (unsigned __int128)n * i;
        unsigned cnt = popcount_uint128(prod);
        if (cnt == i) {
            return (seq_t)i;
        }
    }
    return 0;
}
