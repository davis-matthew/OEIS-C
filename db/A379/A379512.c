/*
 * A379512  Erase digits 0 and 1 from decimal expansion of n. Then keep just
 *          the coprime digits; write 0 if all digits disappear.
 * https://oeis.org/A379512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Repeatedly erase all decimal digits 0 and 1 from the current
   number, then keep only the digits that are coprime with every other
   remaining digit (duplicates are not coprime with themselves).  The
   process is repeated until no digits are removed; the final number
   (or 0 if nothing survives) is returned as a `seq_t`. */

#include "../utils/gcd.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 immediately. */
    if (n == 0)
        return 0;

    /* Current value is kept in the idx_t domain to preserve the
       original decimal digits for extraction.  The resulting number
       will always fit in idx_t because at every step we discard
       digits. */
    idx_t cur = n;

    /* Max decimal digits of a 64‑bit number is 20, so a fixed buffer
       of that size is safe for all idx_t values. */
    uint8_t buf[20];
    bool keep[20];

    while (1) {
        /* Extract digits of cur in reverse order, skipping 0 and 1. */
        int len = 0;
        idx_t tmp = cur;
        while (tmp > 0) {
            uint8_t d = tmp % 10;
            if (d != 0 && d != 1)
                buf[len++] = d;
            tmp /= 10;
        }

        /* If all digits were removed, the result is 0. */
        if (len == 0)
            return 0;

        /* Revert the order to the original decimal order. */
        for (int i = 0; i < len / 2; ++i) {
            uint8_t t = buf[i];
            buf[i]      = buf[len - 1 - i];
            buf[len - 1 - i] = t;
        }

        /* Determine which digits are coprime to all others. */
        for (int i = 0; i < len; ++i)
            keep[i] = true;
        for (int i = 0; i < len; ++i) {
            if (!keep[i])
                continue;
            for (int j = 0; j < len; ++j) {
                if (i == j)
                    continue;
                if (gcd((seq_t)buf[i], (seq_t)buf[j]) != 1) {
                    keep[i] = false;
                    break;
                }
            }
        }

        /* Rebuild the next number from the digits that survived. */
        idx_t next = 0;
        bool any   = false;
        for (int i = 0; i < len; ++i) {
            if (keep[i]) {
                next = next * 10 + buf[i];
                any = true;
            }
        }

        /* If no digits survived, the result is 0. */
        if (!any)
            return 0;

        /* If we have reached a fixed point, return the value. */
        if (next == cur)
            return (seq_t)next;

        /* Otherwise continue the process with the new number. */
        cur = next;
    }
}
