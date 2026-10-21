/*
 * A297772  Number of distinct runs in base-4 digits of n.
 * https://oeis.org/A297772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Computes the number of distinct runs in the base‑4 digit representation of n.
   The representation is obtained by successively dividing by 4.  A run is a
   maximal sequence of equal digits; the function counts how many different
   (digit, run‑length) pairs appear in that representation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    /* Base‑4 digits of n, least significant digit first.  For a 64‑bit n
       there are at most 32 base‑4 digits. */
    uint8_t digits[32];
    size_t dcnt = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        digits[dcnt++] = (uint8_t)(tmp & 3ULL);   /* tmp % 4 */
        tmp >>= 2;                               /* tmp / 4 */
    }

    /* Table of seen (digit, run‑length) pairs.
       Since dcnt ≤ 32, maximum run length is 32. */
    bool seen[4][33] = {0};

    seq_t distinct = 0;

    /* Scan the digits from most to least significant to discover runs. */
    int i = (int)dcnt - 1;
    while (i >= 0) {
        uint8_t digit = digits[i];
        int len = 1;
        i--;
        while (i >= 0 && digits[i] == digit) {
            len++;
            i--;
        }
        if (!seen[digit][len]) {
            seen[digit][len] = true;
            distinct++;
        }
    }

    return distinct;
}
