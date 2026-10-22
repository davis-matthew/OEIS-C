/*
 * A347346  Smallest palindrome ending with n or 0 if n is a multiple of 10.
 * https://oeis.org/A347346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest palindrome ending with the decimal *
 *            representation of n; a(n) = 0 if n is a multiple of 10. * The
 *            following implementation searches for the minimal palindrome *
 *            length and constructs the palindrome directly
 * Range:     u8 n <= 12, u16 n <= 106, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Multiple of 10 gives 0. */
    if (n % 10 == 0)
        return 0;

    /* Extract decimal digits of n (most significant first). */
    idx_t tmp = n;
    uint8_t rev[64];          /* digits in reverse order, least->most */
    int d = 0;
    do {
        rev[d++] = tmp % 10;
        tmp /= 10;
    } while (tmp > 0);
    uint8_t s[64];            /* most significant first */
    for (int i = 0; i < d; ++i)
        s[i] = rev[d - 1 - i];

    /* Try lengths from d up to 2*d.  For any n the palindrome
     * reverse(s)+s has length 2*d, so a solution exists in this range. */
    for (int L = d; L <= 2 * d; ++L) {
        int pal[128];
        for (int i = 0; i < L; ++i)
            pal[i] = -1;                 /* unassigned */

        /* Set suffix to n. */
        for (int i = 0; i < d; ++i)
            pal[L - d + i] = s[i];

        /* Propagate palindrome constraints. */
        bool conflict = false;
        bool changed  = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < L; ++i) {
                int j = L - 1 - i;
                if (pal[i] != -1 && pal[j] == -1) {
                    pal[j] = pal[i];
                    changed = true;
                } else if (pal[i] == -1 && pal[j] != -1) {
                    pal[i] = pal[j];
                    changed = true;
                } else if (pal[i] != -1 && pal[j] != -1 && pal[i] != pal[j]) {
                    conflict = true;
                    changed  = false;
                    break;
                }
            }
            if (conflict)
                break;
        }
        if (conflict)
            continue;          /* try longer length */

        /* Assign remaining digits to the minimal values. */
        if (pal[0] == -1)   /* most significant digit */
            pal[0] = 1;     /* cannot be zero */
        for (int i = 1; i < L; ++i)
            if (pal[i] == -1)
                pal[i] = 0;

        /* Convert the digit array to seq_t value. */
        seq_t val = 0;
        for (int i = 0; i < L; ++i)
            val = val * 10 + pal[i];

        return val;
    }

    /* Should never reach here for valid inputs. */
    return 0;
}
