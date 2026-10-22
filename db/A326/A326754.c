/*
 * A326754  BII-numbers of set-systems covering an initial interval of
 *          positive integers.
 * https://oeis.org/A326754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   covers_initial_interval
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nth BII-number of set‑systems covering an initial interval of positive integers.
 * The function iterates over successive non‑negative integers, tests each for
 * the covering property, and returns the *n*th one (a(1)=0). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/covers_initial_interval.h"

/* Return the nth BII‑number (1‑based indexing). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t cur   = 0;          /* test non‑negative integers in order */
    while (found < n)
    {
        if (covers_initial_interval(cur))
        {
            ++found;
            if (found == n)
                return (seq_t)cur;
        }
        ++cur;
    }
    /* Unreachable, but silence the compiler. */
    return 0;
}
