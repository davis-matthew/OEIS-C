/*
 * A334752  a(n) is the number immediately below n in a clockwise square
 *          spiral of the positive integers with the first step to the right.
 * https://oeis.org/A334752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number immediately below n in a clockwise square
 *            spiral of positive integers starting with a step to the right.
 *            Each element is found by converting n to its grid coordinate,
 *            moving one step down, and converting the new coordinate back to
 *            a number
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   coord_to_value, value_to_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/coord_to_value.h"
#include "../utils/value_to_coord.h"

/* Main sequence function */
seq_t a(idx_t n) {
    long long x, y;
    value_to_coord(n, &x, &y);          /* coordinates of n */
    return coord_to_value(x, y - 1);    /* number immediately below */
}
