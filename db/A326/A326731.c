/*
 * A326731  a(0) = 0; for n >= 1, a(n) = result of inverting s-th bit (from
 *          left) in n, where s is the number of ones in the binary
 *          representation of n.
 * https://oeis.org/A326731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n XOR (1ULL << (bit_length(n) - popcount(n))) for n≥1,
 *            a(0)=0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
static inline unsigned popcount64(idx_t x) {
    return __builtin_popcountll((unsigned long long)x);
}
static inline unsigned bitlen64(idx_t x) {
    /* x > 0 */
    return 64 - __builtin_clzll((unsigned long long)x);
}
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    unsigned pc   = popcount64(n);
    unsigned bl   = bitlen64(n);
    unsigned shift = bl - pc;
    uint64_t mask   = (uint64_t)1 << shift;
    return (seq_t)n ^ (seq_t)mask;
}
