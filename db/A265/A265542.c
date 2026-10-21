/*
 * A265542  Largest base-9 palindrome m <= n, written in base 9.
 * https://oeis.org/A265542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest base‑9 palindrome m <= n, written in base 9
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Largest base‑9 palindrome m ≤ n, written in base 9.  */
seq_t a(idx_t n)
{
    /* Step 1: convert n to base‑9 digits (most significant first).  */
    int digits[64];  /* enough for n < 9^64 */
    int len = 0;
    if (n == 0)
    {
        digits[0] = 0;
        len = 1;
    }
    else
    {
        idx_t tmp = n;
        while (tmp > 0 && len < 64)
        {
            digits[len++] = tmp % 9;
            tmp /= 9;
        }
        /* reverse to most significant first */
        for (int i = 0; i < len / 2; ++i)
        {
            int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    int left_len = (len + 1) / 2;
    int prefix[64];
    for (int i = 0; i < left_len; ++i)
        prefix[i] = digits[i];

    /* Build palindrome digits from current prefix.  */
    int pal[64];
    for (int i = 0; i < len; ++i)
    {
        if (i < left_len)
            pal[i] = prefix[i];
        else
            pal[i] = prefix[len - 1 - i];
    }

    /* Compute m_val = value of the palindrome in base 9.  */
    seq_t m_val = 0;
    for (int i = 0; i < len; ++i)
        m_val = m_val * (seq_t)9 + (seq_t)pal[i];

    if (m_val <= (seq_t)n)
    {
        /* Convert palindrome digits to output value (decimal digits).  */
        seq_t out_val = 0;
        for (int i = 0; i < len; ++i)
            out_val = out_val * (seq_t)10 + (seq_t)pal[i];
        return out_val;
    }

    /* Decrement the left half (in base 9).  */
    for (int i = left_len - 1; i >= 0; --i)
    {
        if (prefix[i] > 0)
        {
            prefix[i]--;
            break;
        }
        else
        {
            prefix[i] = 8;  /* borrow continues */
        }
    }

    /* If after decrement the leading digit is zero, we must reduce length.  */
    if (prefix[0] == 0)
    {
        int new_len = len - 1;  /* len ≥ 2 here */
        seq_t out_val = 0;
        for (int i = 0; i < new_len; ++i)
            out_val = out_val * (seq_t)10 + (seq_t)8;  /* all 8's */
        return out_val;
    }

    /* Rebuild palindrome with the decremented prefix.  */
    for (int i = 0; i < len; ++i)
    {
        if (i < left_len)
            pal[i] = prefix[i];
        else
            pal[i] = prefix[len - 1 - i];
    }

    /* Output the palindrome digits as decimal.  */
    seq_t out_val = 0;
    for (int i = 0; i < len; ++i)
        out_val = out_val * (seq_t)10 + (seq_t)pal[i];
    return out_val;
}
