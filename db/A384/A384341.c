/*
 * A384341  Consecutive states of the linear congruential pseudo-random number
 *          generator (31481*s+21139) mod 10^5 when started at s=1.
 * https://oeis.org/A384341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (31481 * a(n‑1) + 21139) mod 100000 (n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define PERIOD 100000

seq_t a(idx_t n)
{
    /* static array to hold one full period of the generator;
       trivial zero‑initialisation is sufficient for all supported
       seq_t types (built‑in unsigned integers or arbitrary‑precision
       integer classes that can be constructed from zero or from a
       literal). */
    static seq_t cache[PERIOD + 1] = {0};
    static int init = 0;

    if (!init) {
        init = 1;
        cache[1] = (seq_t)1;
        for (idx_t i = 2; i <= PERIOD; ++i) {
            /* all intermediate values fit in 64 bits because
               cache[i-1] < 100000. */
            uint64_t prev = (uint64_t)cache[i - 1];
            uint64_t tmp  = prev * 31481ULL + 21139ULL;
            cache[i] = (seq_t)(tmp % 100000ULL);
        }
    }

    /* wrap the index modulo the period (a(1) through a(100000) are
       stored in cache[1 .. 100000]). */
    idx_t idx = ((n - 1) % PERIOD) + 1;
    return cache[idx];
}
