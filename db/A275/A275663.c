/*
 * A275663  Number of squares in {n, f(n), f(f(n)), ...., 1}, where f is the
 *          Collatz function.
 * https://oeis.org/A275663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Count the number of perfect square terms in the Collatz
   trajectory of n (including n itself and the terminal 1). */

static inline bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t curr = (seq_t)n;          /* start of the trajectory */

    for (;;)
    {
        if (is_square(curr))
            ++count;
        if (curr == (seq_t)1)
            break;

        /* Transitions of the Collatz map */
        if ((curr & (seq_t)1) == 0)   /* even */
            curr = curr >> 1;        /* curr / 2 */
        else                         /* odd  */
            curr = curr * (seq_t)3 + (seq_t)1;
    }

    return count;
}
