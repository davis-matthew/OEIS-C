/*
 * A297147  Numbers having a down-first zigzag pattern in base 10; see
 *          Comments.
 * https://oeis.org/A297147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Down‑first zigzag in base 10. Computed by brute‑force search
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: check whether a number has a down‑first zigzag pattern in base 10. */
static bool has_down_first_zigzag(seq_t num)
{
    /* maximal decimal digits for 128‑bit integer: 39 */
    uint8_t digits[40];
    int len = 0;
    seq_t tmp = num;

    /* extract digits least‑significant first */
    do
    {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp != 0);

    /* at least two digits required */
    if (len < 2)
        return false;

    uint8_t prev = digits[len - 1]; /* most significant digit */
    int prev_dir = 0;               /* 1 = up, -1 = down */

    /* scan from second most significant digit down to the units digit */
    for (int i = len - 2; i >= 0; --i)
    {
        uint8_t cur = digits[i];
        if (cur == prev)                 /* equal adjacent digits not allowed */
            return false;

        if (prev_dir == 0)                /* first comparison: must be down */
        {
            if (cur >= prev)              /* should be strictly smaller */
                return false;
            prev_dir = -1;                /* start with a down segment */
        }
        else
        {
            int dir = (cur > prev) ? 1 : -1;
            if (dir != prev_dir)          /* direction changed */
                prev_dir = dir;           /* new segment starts */
        }
        prev = cur;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 10;                  /* start from the smallest two‑digit number */

    while (1)
    {
        if (has_down_first_zigzag(num))
        {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }
}
