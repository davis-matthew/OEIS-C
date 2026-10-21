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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

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
