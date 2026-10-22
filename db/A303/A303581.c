/*
 * A303581  Add i (>= 0) to the i-th block of terms in the Thue-Morse sequence
 *          A010060.
 * https://oeis.org/A303581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (popcount(n) mod 2) + ceil(log2(n+1)) where popcount(n)
 *            is the number of 1-bits in n (Thue‑Morse)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   popcount_parity_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcount_parity_u64.h"

/* floor(log2(x)) for x > 0  (result 0..63) */
static inline unsigned floor_log2_u64(uint64_t x) {
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    unsigned parity = popcount_parity_u64(n);
    unsigned offset = (n == 0) ? 0 : floor_log2_u64(n) + 1U;  /* ceil(log2(n+1)) */
    return (seq_t)(parity + offset);
}
