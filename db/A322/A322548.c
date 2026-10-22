/*
 * A322548  Integers x such that x^2 + 119 = 15*2^y.
 * https://oeis.org/A322548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322548: integers x such that x^2 + 119 = 15·2^y    (only 6 solutions) */
#include <stddef.h>

static const seq_t _solutions[6] = {1, 11, 19, 29, 61, 701};

seq_t a(idx_t n)
{
    if (n < 1 || n > 6) return 0;
    return _solutions[n - 1];
}
