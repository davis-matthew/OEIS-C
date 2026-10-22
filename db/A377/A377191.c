/*
 * A377191  Smallest number of digits that must be changed in n to obtain a
 *          palindrome (without changing the first digit to 0).
 * https://oeis.org/A377191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes minimal number of digit changes required to transform n into a palindrome,
   while ensuring the leading digit of the resulting palindrome is non‑zero. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;               /* convert index to seq_t */

    /* Store decimal digits of x in least‑significant first order */
    uint8_t buf[128];
    size_t len = 0;

    if (x == 0) {
        buf[0] = 0;
        len = 1;
    } else {
        seq_t tmp = x;
        while (tmp != 0) {
            buf[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    if (len <= 1)            /* single digit is already a palindrome */
        return 0;

    seq_t changes = 0;

    /* Handle the first digit pair (most and least significant). */
    int left  = buf[len - 1]; /* most significant */
    int right = buf[0];       /* least significant */

    if (left == right) {
        if (left != 0)          /* non‑zero equal digits cost nothing   */
            changes += 0;
        else                    /* both zero: must change both to a non‑zero digit */
            changes += 2;
    } else {
        changes += 1;           /* choose the non‑zero digit as the palindrome digit */
    }

    /* Handle remaining digit pairs. */
    for (size_t i = 1; i < len / 2; ++i) {
        if (buf[len - 1 - i] != buf[i])
            changes += 1;
    }

    /* A middle digit in an odd‑length number costs nothing. */
    return changes;
}
