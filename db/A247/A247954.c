/*
 * A247954  a(n) = sigma(sigma(2n-1)).
 * https://oeis.org/A247954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(sigma(2*n-1))
 * Range:     u8 n <= 42, u16 n <= 4567, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9782 (2026-11-01)
 * Helpers:   sigma_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, etc. */
#include "../utils/sigma_seq.h"

seq_t a(idx_t n) {
    seq_t m = ((seq_t)n << 1) - 1;   /* 2*n - 1 */
    seq_t s = sigma_seq(m);
    return sigma_seq(s);
}
