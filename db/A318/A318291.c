/*
 * A318291  a(n) is the minimum k > 0 such that n*2^k - 3 is prime, or 0 if no
 *          such k exists.
 * https://oeis.org/A318291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 2611, u32 n <= 2611, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimal k>0 such that n*2^k-3 is prime; returns 0 if none found within a safe bound. */
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* out‑of‑range guard, unused for OEIS offset */
    const idx_t MAX_K = 2000;                 /* practical search limit                 */
    const unsigned MAX_BITS = CHAR_BIT * sizeof(seq_t);
    seq_t power = (seq_t)2;                   /* 2^1                               */
    idx_t k = 1;
    while (k <= MAX_K) {
        unsigned b_n = bitlen_seq_t((seq_t)n);
        unsigned b_p = bitlen_seq_t(power);
        if (b_n + b_p > MAX_BITS) break;               /* product would overflow          */
        seq_t prod = (seq_t)n * power;
        seq_t val  = prod - (seq_t)3;
        if (is_prime_big(val)) return (seq_t)k;
        if (b_p + 1 > MAX_BITS) break;                 /* next power would overflow       */
        power <<= 1;
        ++k;
    }
    return (seq_t)0;
}
