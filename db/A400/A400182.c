/*
 * A400182  Number of steps starting from the n-th positive integer not
 *          divisible by 5 until the first repeated value under the reduced
 *          residue-dependent 6k +- {1,2} map, or -1 if no value ever repeats.
 * https://oeis.org/A400182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   map_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A400182 – Number of steps until the first repeated value
    in the reduced residue‑dependent 6k ± {1,2} map.
    The implementation uses Floyd's cycle‑finding algorithm on the
    orbit of b(n) = floor((5*n-1)/4). */

#include <stdbool.h>
#include "../utils/map_T.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)-1;   /* invalid index */

    /* starting value b(n) = floor((5*n-1)/4) */
    seq_t start = ((seq_t)5 * (seq_t)n - 1) / 4;

    /* Floyd's cycle detection */
    seq_t tortoise = map_T(start);
    seq_t hare     = map_T(map_T(start));

    while (tortoise != hare) {
        tortoise = map_T(tortoise);
        hare     = map_T(map_T(hare));
    }

    /* cycle length λ */
    seq_t hare2 = map_T(tortoise);
    idx_t lambda = 1;
    while (tortoise != hare2) {
        hare2 = map_T(hare2);
        ++lambda;
    }

    /* preperiod length μ */
    seq_t tortoise2 = start;
    seq_t hare3     = start;
    for (idx_t i = 0; i < lambda; ++i)
        hare3 = map_T(hare3);
    idx_t mu = 0;
    while (tortoise2 != hare3) {
        tortoise2 = map_T(tortoise2);
        hare3     = map_T(hare3);
        ++mu;
    }

    return (seq_t)(mu + lambda);
}
