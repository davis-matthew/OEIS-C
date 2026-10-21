/*
 * A297281  Numbers whose base-13 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑13 up‑variation exceeds down‑variation */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* offset is 1, but guard anyway */
        return (seq_t)0;

    seq_t cur = (seq_t)15;       /* first term is 15 (base‑13 digits 1,2) */
    idx_t   seen = (idx_t)0;     /* how many qualifying numbers found so far */

    const seq_t base13 = (seq_t)13;   /* base for conversion */

    /* temporary buffer for digits, maximum needed for 128‑bit numbers */
    uint8_t digits[128];

    while (1) {
        /* decompose cur into base‑13 digits (least‑significant first) */
        seq_t tmp = cur;
        size_t len = 0;
        while (tmp > (seq_t)0) {
            digits[len++] = (uint8_t)(tmp % base13);
            tmp /= base13;
        }

        /* compute up‑variation and down‑variation */
        seq_t up   = (seq_t)0;
        seq_t down = (seq_t)0;

        if (len > 1) {
            seq_t prev = (seq_t)digits[len - 1];
            for (int i = (int)len - 2; i >= 0; --i) {
                seq_t cur_digit = (seq_t)digits[i];
                if (cur_digit > prev)
                    up += cur_digit - prev;
                else
                    down += prev - cur_digit;
                prev = cur_digit;
            }
        }

        if (up > down) {
            ++seen;
            if (seen == n)
                return cur;   /* found the n‑th term */
        }

        ++cur;      /* next candidate */
    }
}
