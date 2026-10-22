/*
 * A342846  Number of distinct odd numbers visible as proper substrings of n.
 * https://oeis.org/A342846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9834, u16 n <= 9834, u32 n <= 9834, u64 n <= 9834,
 *            u128 n <= 9834, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A342846: Number of distinct odd numbers visible as proper substrings of n.
   This implementation enumerates all proper substrings of n, collects the
   distinct odd values, and returns their count.  The maximum number of
   proper substrings for a decimal string of length ≤20 is 190, so a fixed
   array of size 200 suffices. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to a decimal string stored in digits[0..len-1]
       (most significant digit first).  A 64‑bit integer has at most 20
       decimal digits, so 21 bytes is safe. */
    char digits[21];
    int len = 0;
    idx_t tmp = n;
    do
    {
        digits[len++] = (char)('0' + (int)(tmp % 10));
        tmp /= 10;
    } while (tmp > 0);
    /* Reverse to obtain the natural-order string. */
    for (int i = 0; i < len / 2; ++i)
    {
        char tmpc = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmpc;
    }

    /* Store seen odd values (idx_t is large enough to hold any substring). */
    idx_t seen[200];
    size_t seen_cnt = 0;

    for (int start = 0; start < len; ++start)
    {
        idx_t val = 0;
        for (int end = start; end < len; ++end)
        {
            int sublen = end - start + 1;
            if (sublen == len)          /* skip the full number itself */
                break;
            val = val * (idx_t)10 + (idx_t)(digits[end] - '0');
            if ((val & (idx_t)1) == 0)  /* even value -> skip */
                continue;
            /* Check for repetition among already seen odd values. */
            bool duplicate = false;
            for (size_t k = 0; k < seen_cnt; ++k)
                if (seen[k] == val)
                {
                    duplicate = true;
                    break;
                }
            if (!duplicate)
                seen[seen_cnt++] = val;
        }
    }
    return (seq_t)seen_cnt;
}
