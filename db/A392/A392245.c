/*
 * A392245  Number computed by Wolfram's 2-state 2-symbol Turing machine 1285
 *          when started with n on the tape.
 * https://oeis.org/A392245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - (n mod 2^k) where k = min{ k >= 0 | n & (3 * 2^k) = 0
 *            } (OEIS A392245)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = 0;
    /* Find smallest k such that bits k and k+1 of n are both zero */
    for (; k < 64; ++k) {
        if (((n >> k) & 3ULL) == 0ULL) {
            break;
        }
    }
    /* If no such k within 0..63, then k must be 64 (n fits in 64 bits) */
    if (k == 64) {
        /* 2^64 > n, so n mod 2^k = n, hence a(n) = 0 */
        return (seq_t)0;
    }
    seq_t sn   = (seq_t)n;
    seq_t mask = (((seq_t)1 << k) - 1);
    seq_t rem  = sn & mask;      /* n mod 2^k */
    return sn - rem;             /* a(n) = n - rem */
}
