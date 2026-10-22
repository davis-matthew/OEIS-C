/*
 * A369235  The 180-degree rotations of those numbers which can be rotated 180
 *          degrees, where digits 2 and 7 are 180-degree rotations of each
 *          other.
 * https://oeis.org/A369235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 180° rotation of the n‑th natural number that consists
 *            only of the digits 0,1,2,6,7,8,9. The function builds that n‑th
 *            number in increasing order and returns its 180° rotation,
 *            stripping any leading zeros in the result
 * Range:     u8 n <= 50, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t first_digits[6] = {1, 2, 6, 7, 8, 9};
static const uint8_t all_digits[7]  = {0, 1, 2, 6, 7, 8, 9};
static const uint8_t rot_map[10]    = {0, 1, 7, 0, 0, 0, 9, 2, 8, 6};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* n-1 gives a zero‑based index among the positive rotatable numbers. */
    idx_t idx = n - 1;

    /* Find the length of the number that contains this index. */
    idx_t power = 1;          /* 7^(len-1) */
    idx_t len   = 1;
    for (;;)
    {
        idx_t count = 6 * power;           /* how many numbers of this length */
        if (idx < count)
            break;                      /* found the correct length */
        idx -= count;
        power *= 7;
        ++len;
    }

    /* idx now is the rank within numbers of length 'len'.  Determine first digit. */
    idx_t first_idx = idx / power;      /* 0..5 */
    idx_t rem       = idx % power;      /* remainder for the remaining (len-1) digits */

    /* Build the 180° rotation – process digits from least significant to most. */
    seq_t rotated = 0;

    /* Process the digits that are not the most significant one (there are len-1 of them). */
    for (idx_t i = 0; i < len - 1; ++i)
    {
        uint8_t digit_idx = rem % 7;    /* 0..6 for the current position */
        rem /= 7;
        uint8_t digit = all_digits[digit_idx];
        rotated = rotated * 10 + rot_map[digit];
    }

    /* Finally process the most significant digit. */
    uint8_t msd = first_digits[first_idx];
    rotated = rotated * 10 + rot_map[msd];

    return rotated;
}
