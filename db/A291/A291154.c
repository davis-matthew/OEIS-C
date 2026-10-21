/*
 * A291154  Red numbers on the roulette wheel.
 * https://oeis.org/A291154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth red roulette number (sorted ascending)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence contains the 18 red numbers on a standard roulette wheel:
   1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36. */

#include <stddef.h>
#include <stdbool.h>

/* The return type is seq_t (unsigned integer type, may be arbitrary precision).
   The index type is idx_t (unsigned 64‑bit integer). */
seq_t a(idx_t n) {
    if (n == 0)   /* index 0 is outside the defined offset */
        return (seq_t)0;

    /* Boolean table indicating whether a number 1..36 is red.
       Index 0 is unused (the green 0 on the wheel). */
    static const bool is_red[37] = {
        false, /* 0: green */
        true,  /* 1 */
        false, /* 2 */
        true,  /* 3 */
        false, /* 4 */
        true,  /* 5 */
        false, /* 6 */
        true,  /* 7 */
        false, /* 8 */
        true,  /* 9 */
        false, /*10*/
        false, /*11*/
        true,  /*12*/
        false, /*13*/
        true,  /*14*/
        false, /*15*/
        true,  /*16*/
        false, /*17*/
        true,  /*18*/
        true,  /*19*/
        false, /*20*/
        true,  /*21*/
        false, /*22*/
        true,  /*23*/
        false, /*24*/
        true,  /*25*/
        false, /*26*/
        true,  /*27*/
        false, /*28*/
        false, /*29*/
        true,  /*30*/
        false, /*31*/
        true,  /*32*/
        false, /*33*/
        true,  /*34*/
        false, /*35*/
        true   /*36*/
    };

    /* Count red numbers until we hit the nth one. */
    idx_t count = 0;
    for (idx_t i = 1; i <= 36; ++i) {
        if (is_red[(size_t)i]) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }

    /* If n > 18 the sequence is undefined; return 0. */
    return (seq_t)0;
}
