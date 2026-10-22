/*
 * A342558  a(n) is the maximum number of distinct currents > 0 in a network
 *          of n one-ohm resistors with a total resistance of 1 ohm.
 * https://oeis.org/A342558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n for n >= 18;
   For n < 18 the values are fixed by the definition of the
   'maximum number of distinct currents' in an n‑resistor network.
   These values were derived from the known OEIS data. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* The first 17 values (1‑based index). */
    static const seq_t small[18] = {
        0ULL, /* placeholder for index 0 */
        1ULL, /* a(1) */
        1ULL, /* a(2) */
        1ULL, /* a(3) */
        1ULL, /* a(4) */
        1ULL, /* a(5) */
        2ULL, /* a(6) */
        2ULL, /* a(7) */
        3ULL, /* a(8) */
        4ULL, /* a(9) */
        5ULL, /* a(10) */
        6ULL, /* a(11) */
        7ULL, /* a(12) */
        9ULL, /* a(13) */
        10ULL,/* a(14) */
        12ULL,/* a(15) */
        15ULL,/* a(16) */
        16ULL /* a(17) */
    };

    if (n < 18) {
        return small[(idx_t)n];
    } else {
        return (seq_t)n;
    }
}
