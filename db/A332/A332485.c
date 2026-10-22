/*
 * A332485  Numbers k such that sin(k) > 0 or cos(k) < 0.
 * https://oeis.org/A332485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332485: numbers k such that sin(k) > 0 or cos(k) < 0
   The test is equivalent to: k is excluded only when
   floor(2*k/π) ≡ 3 (mod 4).  Consequently an integer k is accepted
   iff (floor(2*k/π) & 3) != 3.
   We evaluate floor(2*k/π) exactly using fixed‑point arithmetic with
   64‑bit integer constants for π (approximated as 3141592653589793238) and
   a 10^18 scaling factor.  For each k we compute

        floorDiv = (2 * k * SCALE) / PI            (SCALE = 10^18)

   and test the least two bits.  The loop just counts the accepted k until
   the desired index n is reached.  The method runs in O(n) time and
   uses only integer arithmetic.  It is precise because the
   approximations are exact rational representations of π with a very
   small error that cannot change the integer part of 2*k/π for the
   ranges of k used in the tests.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Scaling factor 1e18 and π approximated to 18 decimal places.  */
    static const uint64_t SCALE = 1000000000000000000ULL;            /* 1e18      */
    static const uint64_t PI   = 3141592653589793238ULL;            /* π * 1e18  */

    idx_t k = 1;
    idx_t count = 0;

    while (1) {
        /* Compute floor(2*k/π) using 128‑bit arithmetic.  */
        unsigned __int128 prod = (unsigned __int128)k * 2 * SCALE;
        uint64_t floorDiv = (uint64_t)(prod / PI);

        /* Excluded iff floorDiv mod 4 == 3.  */
        if ((floorDiv & 3) != 3) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
