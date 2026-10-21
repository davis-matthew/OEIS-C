/*
 * A246456  a(n) = sigma(n + sigma(n)).
 * https://oeis.org/A246456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n + sigma(n))
 * Range:     u8 n <= 41, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* First compute sigma(n) */
    seq_t s = sigma_of(n);

    /* Compute n + sigma(n) */
    seq_t sum = (seq_t)n + s;

    /* Finally compute sigma(n + sigma(n)) */
    /* The sum fits into idx_t for all practical n in the test suite. */
    return sigma_of((idx_t)sum);
}
