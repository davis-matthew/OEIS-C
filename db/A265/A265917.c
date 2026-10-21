/*
 * A265917  a(n) = floor(A070939(n)/A000120(n)) where A070939(n) is the binary
 *          length of n and A000120(n) is the binary weight of n.
 * https://oeis.org/A265917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(A070939(n)/A000120(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070939(n) = bit length of n (floor(log2 n) + 1)              */
/* A000120(n) = popcount (number of 1 bits in n)                */

/* Computes a(n) = floor(bitlen(n) / popcount(n)) for n >= 1. */

#include <stdint.h>

/* Helper: compute bit length of a 64‑bit unsigned integer */
static inline idx_t bitlen(idx_t x) {
    idx_t len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

/* Helper: compute popcount of a 64‑bit unsigned integer */
static inline idx_t popcount(idx_t x) {
    /* Use compiler builtin if available, else fallback */
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    /* Portable popcount implementation */
    idx_t count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
#endif
}

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 by the sequence's offset */
    idx_t len = bitlen(n);
    idx_t pop = popcount(n);
    /* pop > 0 for n >= 1 */
    return (seq_t)len / (seq_t)pop;
}
