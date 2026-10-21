/*
 * A294760  Number of permutations of [n] avoiding {4321, 1324, 1234}.
 * https://oeis.org/A294760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = number of permutations of [n] avoiding {4321, 1324, 1234}
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   _avoids, _count_avoiders, _gen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (By Erdos–Szekeres, a(n)=0 for n>=10. The rest is computed by brute force
   enumeration of all permutations up to size 9.) */
#include <stdbool.h>
#include "../utils/_avoids.h"
#include "../utils/_count_avoiders.h"
#include "../utils/_gen.h"

/* Public entry function. */
seq_t a(idx_t n) {
    static bool ready = false;
    static seq_t cache[10];
    if (!ready) {
        /* Compute all values up to n = 9 once. */
        cache[0] = 1;                  /* by definition */
        for (int len = 1; len <= 9; ++len)
            cache[len] = _count_avoiders(len);
        ready = true;
    }
    if (n >= 10) return 0;            /* no avoiders for n >= 10 */
    return cache[(int)n];
}
