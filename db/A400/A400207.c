/*
 * A400207  Sparkly numbers: a positive integer is called sparkly if it has
 *          exactly 9 digits, and for any m between 1 and 9 (inclusive), the
 *          m-th digit is a positive multiple of m.
 * https://oeis.org/A400207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 216, u64 n <= 216, u128 n <= 216,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Enumerates sparkly numbers: for each
   d1 ∈ {1,…,9}, d2 ∈ {2,4,6,8},
   d3 ∈ {3,6,9}, d4 ∈ {4,8},
   build the 9‑digit number d1 d2 d3 d4 5 6 7 8 9. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: undefined for 0 */

    /* number of sparkly numbers */
    const idx_t LIMIT = 216;

    if (n > LIMIT) return 0;        /* undefined beyond the known terms */

    idx_t idx = n - 1;              /* convert to zero‑based index */

    /* digit choices for each position */
    static const uint8_t d1[9] = {1,2,3,4,5,6,7,8,9};
    static const uint8_t d2[4] = {2,4,6,8};
    static const uint8_t d3[3] = {3,6,9};
    static const uint8_t d4[2] = {4,8};

    /* decode the base‑(2,3,4,9) representation */
    const idx_t r4 = idx % 2; idx /= 2;   /* D4 */
    const idx_t r3 = idx % 3; idx /= 3;   /* D3 */
    const idx_t r2 = idx % 4; idx /= 4;   /* D2 */
    const idx_t r1 = idx % 9;             /* D1 */

    const uint8_t dig1 = d1[r1];
    const uint8_t dig2 = d2[r2];
    const uint8_t dig3 = d3[r3];
    const uint8_t dig4 = d4[r4];

    /* build the 9‑digit number by successive *10 + digit */
    seq_t val = 0;
    val = val * 10 + dig1;
    val = val * 10 + dig2;
    val = val * 10 + dig3;
    val = val * 10 + dig4;
    val = val * 10 + 5;
    val = val * 10 + 6;
    val = val * 10 + 7;
    val = val * 10 + 8;
    val = val * 10 + 9;

    return val;
}
