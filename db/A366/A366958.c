/*
 * A366958  Numbers whose difference between the largest and smallest digits
 *          is equal to 1.
 * https://oeis.org/A366958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th natural number whose largest and smallest digits
 *            differ by exactly 1. The sequence starts at a(1)=10. The
 *            algorithm determines the number of digits of `a(n)`, finds the
 *            corresponding digit‐pair (d, d+1) and enumerates the number
 *            within that pair in ascending numeric order. No exte...
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t pow2(idx_t e) { return (idx_t)1 << e; }

seq_t a(idx_t n)
{
    /* Find digit length d such that the cumulative counts of numbers
       with ≤ d digits reach n.  Count of d‑digit numbers is
           count[d] = 17 * (2^{d-1} – 1)   for d ≥ 2.    */
    idx_t d = 2;
    idx_t cum = 0;                     /* numbers with fewer than d digits */
    while (1)
    {
        idx_t pow = pow2(d - 1);       /* 2^{d-1} */
        idx_t cnt = 17 * (pow - 1);    /* count[d] */
        if (cum + cnt >= n) break;
        cum += cnt;
        ++d;
    }

    /* Position within the d‑digit block (0‑based) */
    idx_t t = n - cum - 1;

    /* Pick the digit pair (x, x+1) that contains the t‑th number. */
    idx_t remaining = t;
    int  x = -1;                       /* lower digit of the pair */
    idx_t offset = 0;                  /* offset within the pair */
    for (int xi = 0; xi <= 8; ++xi)
    {
        idx_t cnt_x;
        if (xi == 0)
            cnt_x = (pow2(d - 1) - 1);        /* 2^{d-1} – 1 */
        else
            cnt_x = (pow2(d) - 2);           /* 2^{d} – 2 */

        if (remaining < cnt_x)
        {
            x = xi;
            offset = remaining;
            break;
        }
        remaining -= cnt_x;
    }

    /* Build the result number.  The first digit and the remaining
       digits are constructed lexicographically. */
    if (x == 0)
    {
        /* Pair {0,1}.  Leading digit must be 1.  Remaining d‑1 digits
           form a binary pattern of length d‑1 that is not all 1s. */
        idx_t bitcount = d - 1;
        idx_t pat = offset;                    /* 0 … 2^{bitcount}‑2 */
        seq_t num = 1;                         /* leading 1 */
        for (idx_t i = 0; i < bitcount; ++i)
        {
            idx_t bit = (pat >> (bitcount - 1 - i)) & 1;
            seq_t digit = bit ? (seq_t)1 : (seq_t)0;
            num = num * 10 + digit;
        }
        return num;
    }
    else
    {
        /* Pair {x, x+1} where x ≥ 1.  Numbers are split into
           two groups: leading digit = x, or leading digit = x+1.
           Each group has size (2^{d‑1} – 1). */
        idx_t bitcount = d - 1;
        idx_t group_size = pow2(bitcount) - 1;  /* 2^{d‑1} – 1 */

        idx_t r = offset;
        int group;        /* 0 for leading=x, 1 for leading=x+1 */
        if (r < group_size)
            group = 0;
        else
        {
            group = 1;
            r -= group_size;
        }

        seq_t first_digit = (group == 0 ? (seq_t)x : (seq_t)(x + 1));
        idx_t pat;
        if (group == 0)
        {
            /* Exclude bit-pattern all 0s. */
            pat = r + 1;                       /* 1 … 2^{bitcount}‑1 */
        }
        else
        {
            /* Exclude bit-pattern all 1s. */
            pat = r;                           /* 0 … 2^{bitcount}‑2 */
        }

        seq_t num = first_digit;
        for (idx_t i = 0; i < bitcount; ++i)
        {
            idx_t bit = (pat >> (bitcount - 1 - i)) & 1;
            seq_t digit = bit ? (seq_t)(x + 1) : (seq_t)x;
            num = num * 10 + digit;
        }
        return num;
    }
}
