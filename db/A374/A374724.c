/*
 * A374724  Quartet paradiddle Thue-Morse, with axiom {1} and morphism
 *          1->{1,2,3,4,1,1}, 2->{2,1,4,3,2,2}, 3->{3,4,1,2,3,3},
 *          4->{4,3,2,1,4,4}.
 * https://oeis.org/A374724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sequence A374724 is the fixed point of the morphism * 1 →
 *            {1,2,3,4,1,1}, 2 → {2,1,4,3,2,2}, 3 → {3,4,1,2,3,3}, 4 →
 *            {4,3,2,1,4,4} * with axiom 1
 * Range:     u8 n <= 9604, u16 n <= 9604, u32 n <= 9604, u64 n <= 9604,
 *            u128 n <= 9604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint8_t map[4][6] = {
    {0,1,2,3,0,0},   /* 1 -> 1,2,3,4,1,1 */
    {1,0,3,2,1,1},   /* 2 -> 2,1,4,3,2,2 */
    {2,3,0,1,2,2},   /* 3 -> 3,4,1,2,3,3 */
    {3,2,1,0,3,3}    /* 4 -> 4,3,2,1,4,4 */
};

seq_t a(idx_t n)
{
    /* Convert n to base‑6 digits (least significant first) */
    uint8_t digits[64];
    size_t dcount = 0;
    idx_t temp = n;

    if (temp == 0) {
        digits[0] = 0;
        dcount = 1;
    } else {
        while (temp > 0) {
            digits[dcount++] = temp % 6;
            temp /= 6;
        }
    }

    /* Traverse the substitution tree */
    uint8_t symbol = 0;   /* start with '1' (index 0) */

    for (size_t i = dcount; i-- > 0; ) {
        symbol = map[symbol][digits[i]];
    }

    return (seq_t)(symbol + 1);   /* map back to 1..4 */
}
