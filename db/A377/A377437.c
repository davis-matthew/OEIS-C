/*
 * A377437  Number of multiplication steps for n to reach 1 when iterating
 *          5x+1 removing factors 2 and 3, or -1 if it never reaches 1.
 * https://oeis.org/A377437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5x+1 multiplication steps needed for n * to
 *            reach 1 after repeatedly removing all factors of 2 and 3 *
 *            (return -1 if it never reaches 1). This is computed by iterating
 *            * the map: x = odd_part(x) (remove 2,3); if x==1 stop, * else x
 *            = 5*x+1 and repeat. Steps count the number of 5x...
 * Range:     u8 n <= 22, u16 n <= 9929, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   remove23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/remove23.h"

/* OEIS A377437: number of 5x+1 steps to reach 1 */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t steps = 0;

    /* If n itself already reduces to 1, answer is zero. */
    x = remove23(x);
    if (x == 1) return steps;

    while (true) {
        x = 5 * x + 1;     /* multiplication step */
        steps++;
        x = remove23(x);
        if (x == 1) break;
    }

    return steps;
}
