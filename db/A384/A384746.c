/*
 * A384746  Consecutive states of the linear congruential pseudo-random number
 *          generator MCNP from Los Alamos when started at 1.
 * https://oeis.org/A384746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 5^19 * a(n‑1) mod 2^48 for n > 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t constants */

static const seq_t MULTIPLIER = (seq_t)19073486328125ULL;          /* 5^19 */
static const seq_t MODMASK    = (seq_t)((1ULL << 48) - 1);          /* 2^48 - 1 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* guard for accidental n < 1 */

    seq_t result = (seq_t)1;              /* a(1) */

    /* iterate from 2 to n, applying the recurrence */
    for (idx_t i = 1; i < n; ++i) {
        result = (result * MULTIPLIER) & MODMASK;
    }

    return result;
}
