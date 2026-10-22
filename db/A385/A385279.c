/*
 * A385279  Consecutive states of the linear congruential pseudo-random number
 *          generator (625*s + 6571) mod 31104 when started at s=1.
 * https://oeis.org/A385279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (625 * a(n-1) + 6571) mod 31104, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined below offset */

    static seq_t cache[31105] = {0};/* indices 1..31104 inclusive */
    static bool init = false;

    if (!init) {
        cache[1] = 1;
        for (idx_t i = 2; i <= 31104; ++i) {
            cache[i] = ( ( (unsigned long long)625 * cache[i-1] + 6571 ) % 31104 );
        }
        init = true;
    }

    /* reduce n modulo the period 31104, keeping the first element at index 1 */
    idx_t k = ( (n - 1) % 31104ULL ) + 1;
    return cache[k];
}
