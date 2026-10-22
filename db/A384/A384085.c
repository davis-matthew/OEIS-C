/*
 * A384085  Consecutive states of the linear congruential pseudo-random number
 *          generator (40*s+725) mod 729 when started at s=1.
 * https://oeis.org/A384085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (40*a(n-1)+725) mod 729, a(1)=1, periodic with period 729
 * Range:     u8 n <= 2, u16 n <= 729, u32 n <= 729, u64 n <= 729,
 *            u128 n <= 729, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t cache[729];
    static bool init = false;
    if (!init) {
        init = true;
        cache[0] = 1;
        for (idx_t i = 1; i < 729; ++i) {
            cache[i] = ((seq_t)40 * cache[i - 1] + 725) % 729;
        }
    }
    idx_t idx = (n - 1) % 729;
    return cache[idx];
}
