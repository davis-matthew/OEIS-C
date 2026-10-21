/*
 * A265568  Smallest base-7 palindrome m >= n, written in base 10.
 * https://oeis.org/A265568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: returns the smallest integer m ≥ n such that the base‑7
 * representation of m is a palindrome.  The function works in O(log₇ n)
 * time and uses only basic integer arithmetic.
 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n == 0 is a palindrome itself */
    if (n == 0)
        return (seq_t)0;

    /* Step 1: convert n to base‑7 digits (most significant first). */
    int dig[64];            /* enough for 64‑bit numbers (≤ 23 digits) */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        dig[len++] = tmp % 7;
        tmp /= 7;
    }

    /* reverse to get msd at index 0 */
    for (int i = 0; i < len / 2; ++i) {
        int t = dig[i];
        dig[i] = dig[len - 1 - i];
        dig[len - 1 - i] = t;
    }

    /* Step 2: build a palindrome by mirroring the first half. */
    int pal[64];
    for (int i = 0; i < len; ++i) {
        if (i < len / 2)
            pal[i] = dig[i];
        else
            pal[i] = dig[len - 1 - i];
    }

    /* Compare palindrome with original number (lexicographically). */
    int cmp = 0;  /* 0: equal, 1: greater, -1: smaller */
    for (int i = 0; i < len; ++i) {
        if (pal[i] > dig[i]) { cmp = 1; break; }
        if (pal[i] < dig[i]) { cmp = -1; break; }
    }

    if (cmp < 0) {
        /* Palindrome is smaller; increment the left half. */
        int k = (len + 1) / 2;       /* number of digits in left half */
        int carry = 1;
        int pos = k - 1;
        while (pos >= 0 && carry) {
            int v = dig[pos] + carry;
            carry = v / 7;
            dig[pos] = v % 7;
            --pos;
        }

        if (carry) {
            /* Left half overflowed; create the smallest longer palindrome. */
            int new_len = len + 1;
            for (int i = 0; i < new_len; ++i) {
                pal[i] = (i == 0 || i == new_len - 1) ? 1 : 0;
            }
            len = new_len;
        } else {
            /* Mirror again with updated left half. */
            for (int i = 0; i < len; ++i) {
                if (i < len / 2)
                    pal[i] = dig[i];
                else
                    pal[i] = dig[len - 1 - i];
            }
        }
    }

    /* Step 3: convert the palindrome back to decimal (seq_t). */
    seq_t value = 0;
    for (int i = 0; i < len; ++i) {
        value = value * 7 + (seq_t)pal[i];
    }

    return value;
}
