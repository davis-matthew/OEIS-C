/*
 * A366520  Largest odd divisor of n which is < sqrt(n), for n >= 2; a(1) = 1.
 * https://oeis.org/A366520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest odd divisor of n that is < sqrt(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit values (floor). */
static idx_t isqrt_u64(idx_t n) {
    idx_t res  = 0;
    idx_t bit = 1ULL << 32;          /* maximum possible sqrt for 64‑bit n */
    while (bit) {
        idx_t next = res + bit;
        /* use 128‑bit arithmetic to avoid overflow during the test */
        if ((unsigned __int128)next * next <= (unsigned __int128)n) {
            res = next;
        }
        bit >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;

    idx_t sqrt_n = isqrt_u64(n);

    /* Search downward from sqrt(n) for the largest odd divisor */
    for (idx_t d = sqrt_n; d != 0; ) {
        if ((d & 1ULL) && (n % d == 0)) {
            /* ensure d < sqrt(n) using strict inequality */
            if ((unsigned __int128)d * d < (unsigned __int128)n) {
                return (seq_t)d;
            }
        }
        if (d == 1) break; /* avoid underflow of unsigned d */
        d--;
    }
    return (seq_t)1;      /* fallback (only reachable when n = 2) */
}
