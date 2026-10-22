/*
 * A344851  a(n) = (n^2) mod (2^A070939(n)).
 * https://oeis.org/A344851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n^2) mod (2^A070939(n))
 * Range:     u8 n <= 271, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070939(n) is the number of binary digits of n ( = floor(log2(n))+1 for n>0, 0 for n=0). */
/* The sequence is defined for all n ≥ 0, with a(0)=0. */

#include <stdint.h>

/* compute the bit length of an unsigned 64‑bit integer */
static inline int bitlen_uint64(uint64_t x) {
    int len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n) {
    /* a(0) = 0 */
    if (n == 0) {
        return 0;
    }

    /* Determine w = number of binary digits of n */
    const int w = bitlen_uint64((uint64_t)n);

    /* Compute mask = (1 << w) - 1 , handling w==64 safely */
    uint64_t mask;
    if (w == 64) {
        mask = ~0ULL;
    } else {
        mask = (1ULL << w) - 1ULL;
    }

    /* Reduce n modulo 2^w first to avoid overflow */
    uint64_t nmod = (uint64_t)n & mask;

    /* Compute nmod^2 modulo 2^w using 128‑bit arithmetic for safety */
    unsigned __int128 prod = (unsigned __int128)nmod * (unsigned __int128)nmod;
    uint64_t result = (uint64_t)prod & mask;

    return (seq_t)result;
}
