/*
 * A297276  Numbers whose base-12 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9969 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297276: search for numbers whose base‑12 down‑variation
   is greater than its up‑variation. */

#include <stdbool.h>

/* helper: true if n has greater down–variation than up–variation in base 12 */
static bool greater_down_variation(seq_t n)
{
    if (n < (seq_t)12)
        return false;

    unsigned digits[20];
    int len = 0;
    seq_t tmp = n;

    /* extract base‑12 digits (least significant first) */
    while (tmp > 0)
    {
        digits[len++] = (unsigned)(tmp % (seq_t)12);
        tmp /= (seq_t)12;
    }

    seq_t dv = 0, uv = 0;
    for (int i = len - 1; i > 0; --i)
    {
        int diff = (int)digits[i] - (int)digits[i - 1];
        if (diff > 0)
            dv += (seq_t)diff;
        else if (diff < 0)
            uv += (seq_t)(-diff);
    }
    return dv > uv;
}

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    while (1)
    {
        if (greater_down_variation(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
