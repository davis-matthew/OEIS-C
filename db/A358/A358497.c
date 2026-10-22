/*
 * A358497  Replace each new digit in n with index 1, 2, ..., 9, 0 in order in
 *          which that digit appears in n, from left to right.
 * https://oeis.org/A358497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace each new digit in the decimal representation of n
 *            with the next mapping digit 1,2,…,9,0 in the order of first
 *            appearance
 * Range:     u8 n <= 999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0. */
    if (n == 0)
        return 1;

    /* Determine the length of the decimal representation. */
    idx_t t = n;
    idx_t len = 0;
    while (t) {
        ++len;
        t /= 10;
    }

    /* Store the digits from most to least significant in a fixed array. */
    unsigned char digits[20];      /* idx_t is uint64_t, max 20 decimal digits. */
    int pos = 19;
    t = n;
    while (t) {
        digits[pos] = t % 10;
        t /= 10;
        --pos;
    }
    int start = pos + 1;           /* Index of the most significant digit. */

    /* Mapping of original digits to canonical digits. */
    int map[10];
    for (int i = 0; i < 10; ++i)
        map[i] = -1;                /* -1 indicates unmapped. */
    int nextVal = 1;

    /* Build the canonical number. */
    seq_t result = 0;
    for (int i = start; i < 20; ++i) {
        unsigned char d = digits[i];
        if (map[d] == -1) {
            map[d] = nextVal;
            if (nextVal == 9)
                nextVal = 0;        /* After 9 map to 0. */
            else
                ++nextVal;
        }
        result = result * 10 + (seq_t)map[d];
    }

    return result;
}
