/*
 * A256291  Numbers which have only digits 5 and 6 in base 10.
 * https://oeis.org/A256291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number whose digits are the binary *
 *            representation of (n-1), with most significant bit first, *
 *            mapping 0 → 5 and 1 → 6. This yields the increasing sequence *
 *            of base‑10 numbers containing only the digits 5 and 6
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1022, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* Find the length k of the decimal representation.  k = ⌊log₂(n+1)⌋. */
    uint64_t t = n + 1;
    int k = 0;
    while (t > 1) {
        t >>= 1;
        ++k;
    }

    /* 2^k – 2 is the number of all shorter numbers.  Use 128‑bit
     * arithmetic to avoid overflow for very large n. */
    unsigned __int128 pow2k = ((unsigned __int128)1) << k;   /* 2^k   */
    unsigned __int128 before = pow2k - 2;                    /* 2^k-2 */

    /* Zero‑based index inside the k‑digit block. */
    unsigned __int128 idx = ((unsigned __int128)n) - before - 1;

    /* Construct the decimal number digit by digit. */
    seq_t result = 0;
    for (int pos = k - 1; pos >= 0; --pos) {
        unsigned __int128 bit = (idx >> pos) & 1;
        uint8_t digit = bit ? 6 : 5;
        result = result * 10 + digit;
    }
    return result;
}
