/*
 * A384159  Consecutive states of the linear congruential pseudo-random number
 *          generator for 32-bit WATFOR/WATFIV when started at 1.
 * https://oeis.org/A384159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = 20613 * a(n‑1) mod 2^31
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function follows the linear congruential recurrence of the
   WATFOR/WATFIV pseudo‑random number generator described in OEIS
   A384159.  It works for any seq_t that can hold values up to 2^31
   (32‑bit, 64‑bit, 128‑bit or arbitrary precision).  The computation
   uses 64‑bit intermediate arithmetic to keep the product from
   overflowing, then reduces modulo 2^31 via a 64‑bit division.  This is
   O(n) time and uses only a few integer variables, satisfying the
   timing and memory constraints. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = 1;
    const uint64_t MOD = 1ULL << 31;          /* 2^31 */

    for (idx_t i = 2; i <= n; ++i) {
        uint64_t prod = (uint64_t)result * 20613ULL;
        result = (seq_t)(prod % MOD);
    }
    return result;
}
