/*
 * A334971  a(n) is the least prime p such that p+2 is divisible by n-th
 *          prime.
 * https://oeis.org/A334971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that (p + 2) % the n-th prime == 0
 * Range:     u8 n <= 9, u16 n <= 284, u32 n <= 2474, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include <stdint.h>

/* Computes a(n) for the OEIS sequence A334971.  The function assumes
 * n >= 1 (offset 1). For n = 0 it simply returns 0. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* the n-th prime */
    uint32_t q_u32 = get_prime(n);
    seq_t q = (seq_t)q_u32;   /* promote to seq_t for arithmetic */

    /* Special case : n-th prime = 2 */
    if (q_u32 == 2) return (seq_t)2;

    /* Search for the smallest prime of the form p = m*q - 2,  m >= 1 */
    /* Choose a small integer loop counter first; it suffices for n <= 10000 */
    for (uint64_t m = 1; ; ++m) {
        seq_t p = (seq_t)m * q - (seq_t)2;
        if (p < (seq_t)2) continue;   /* skip non‑positive candidates */
        if (is_prime_big(p)) return p;
    }
}
