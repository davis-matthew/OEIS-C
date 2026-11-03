/*
 * A261424  Difference between n and the largest palindrome <= n.
 * https://oeis.org/A261424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 3426, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 114 terms through n = 8825 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261424  
   Formula: a(n) = n - largest palindrome ≤ n
   (largest palindrome is constructed from the decimal digits of n) */

#include <stdbool.h>

/* Find the largest palindrome ≤ n (n is unsigned).  The result
   is returned as seq_t.  The algorithm runs in O(number of digits). */
static seq_t largest_pal_leq(idx_t n)
{
    /* n < 10 → palindrome = n */
    if (n < 10)
        return (seq_t)n;

    /* Extract digits of n into array most‑significant first.  */
    unsigned char digits[64];
    idx_t len = 0;
    idx_t temp = n;
    unsigned char revbuf[64];
    do
    {
        revbuf[len++] = temp % 10;
        temp /= 10;
    } while (temp > 0);

    for (idx_t i = 0; i < len; ++i)
        digits[i] = revbuf[len - 1 - i];

    /* Mirror into a palindrome. */
    for (idx_t i = 0; i < len / 2; ++i)
        digits[len - 1 - i] = digits[i];

    /* Build its numeric value. */
    seq_t pal = 0;
    for (idx_t i = 0; i < len; ++i)
        pal = pal * 10 + digits[i];

    /* If palindrome > n, decrement the middle part. */
    if (pal > n)
    {
        idx_t pos = (len - 1) / 2;           /* left middle for even len */
        while (true)
        {
            if (digits[pos] > 0)
            {
                digits[pos]--;               /* borrow once */
                break;
            }
            digits[pos] = 9;                 /* propagate carry back */
            if (pos == 0)
                break;
            pos--;
        }

        /* Special case: leading digit became 0 → palindrome of
           (len‑1) digits all 9. */
        if (digits[0] == 0)
        {
            pal = 0;
            for (idx_t i = 0; i < len - 1; ++i)
                pal = pal * 10 + 9;
            return pal;
        }

        /* Mirror again after decrement. */
        for (idx_t i = 0; i < len / 2; ++i)
            digits[len - 1 - i] = digits[i];

        pal = 0;
        for (idx_t i = 0; i < len; ++i)
            pal = pal * 10 + digits[i];
    }

    return pal;
}

/* External linkage function required by the problem statement. */
seq_t a(idx_t n)
{
    seq_t pal = largest_pal_leq(n);
    return (seq_t)n - pal;
}
