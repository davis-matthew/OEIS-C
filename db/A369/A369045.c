/*
 * A369045  LCM-transform of binary invert permutation (A054429).
 * https://oeis.org/A369045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime factor of k if k = A054429(n) is a prime power,
 *            otherwise 1. * A054429(n) = 3*2^floor(log2 n) - 1 - n (reversal
 *            within each dyadic block)
 * Range:     u8 n <= 257, u16 n <= 8471, u32 n <= 8471, u64 n <= 8471,
 *            u128 n <= 8471, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find the largest power of two ≤ n */
    uint64_t block = 1;
    while (block <= n) block <<= 1;
    block >>= 1;                     /* now block = 2^⌊log2 n⌋ */

    /* Compute k = A054429(n) = 3*block - 1 - n safely in 128‑bit */
    unsigned __int128 kk = (unsigned __int128)block * 3 - 1 - (uint64_t)n;
    seq_t k = (seq_t)kk;             /* for n ≤ 10000 this fits in 64 bits */

    /* For k ≤ 1 the ratio is always 1 */
    if (k <= 1) return (seq_t)1;

    /* Test whether k is a power of a single prime.
       If yes, return that prime, else return 1. */
    uint64_t m = (uint64_t)k;        /* k fits in 64 bits for the tested range */
    uint64_t temp = m;
    int distinct = 0;
    uint64_t prime = 0;

    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            distinct++;
            if (distinct > 1) return (seq_t)1;   /* more than one prime factor */
            prime = d;
            while (temp % d == 0) temp /= d;
        }
    }

    if (distinct == 0) {          /* m itself is prime */
        return (seq_t)m;
    }

    if (temp == 1) {              /* m was a pure power of the found prime */
        return (seq_t)prime;
    }

    /* Remaining factor ⇒ more than one prime factor */
    return (seq_t)1;
}
