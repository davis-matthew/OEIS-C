/*
 * A265572  Smallest base-9 palindrome m >= n, written in base 10.
 * https://oeis.org/A265572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest base‑9 palindrome m ≥ n (value returned in base 10). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Fast path for trivial palindromes */
    seq_t n_val = (seq_t)n;
    if (is_palindrome(n_val, 9))
        return n_val;

    /* Convert n to base‑9 digits (most significant first). */
    unsigned char msb[32];
    int len = 0;

    if (n == 0) {
        msb[0] = 0;
        len = 1;
    } else {
        unsigned char rev[32];
        int rlen = 0;
        idx_t tmp = n;
        while (tmp > 0) {
            rev[rlen++] = (unsigned char)(tmp % 9);
            tmp /= 9;
        }
        /* Reverse to get msb first. */
        for (int i = 0; i < rlen; ++i)
            msb[i] = rev[rlen - 1 - i];
        len = rlen;
    }

    /* Copy digits into a mutable array. */
    unsigned char p[32];
    for (int i = 0; i < len; ++i)
        p[i] = msb[i];

    /* Make palindrome by mirroring left half to right. */
    for (int i = 0; i < len / 2; ++i)
        p[len - 1 - i] = p[i];

    /* Compute numeric value of the palindrome. */
    seq_t p_val = 0;
    for (int i = 0; i < len; ++i)
        p_val = p_val * 9 + p[i];

    /* If the mirrored palindrome is already greater, return it. */
    if (p_val > n_val)
        return p_val;

    /* Otherwise, increment the middle (or left middle for even length) and propagate carry. */
    int i = (len - 1) / 2;          /* Index of the center or left middle. */
    while (i >= 0) {
        ++p[i];
        if (p[i] < 9)
            break;
        p[i] = 0;
        --i;
    }

    if (i < 0) {  /* Overflow beyond most significant digit. */
        unsigned char newp[33];
        newp[0] = 1;
        for (int k = 1; k < len; ++k)
            newp[k] = 0;
        newp[len] = 1;
        int newlen = len + 1;

        seq_t val = 0;
        for (int k = 0; k < newlen; ++k)
            val = val * 9 + newp[k];
        return val;
    }

    /* Mirror again after increment. */
    for (int j = 0; j < len / 2; ++j)
        p[len - 1 - j] = p[j];

    seq_t val = 0;
    for (int j = 0; j < len; ++j)
        val = val * 9 + p[j];

    return val;
}
