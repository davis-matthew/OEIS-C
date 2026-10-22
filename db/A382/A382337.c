/*
 * A382337  Palindromes in base 10 in which the difference between the sums of
 *          the digits in the even and odd positions is zero.
 * https://oeis.org/A382337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromes in base 10 for which the sum of digits in even
   positions equals the sum of digits in odd positions.  
   The function enumerates palindromes by length, handling even‑
   length palindromes (all of them are valid) and odd‑length
   palindromes (only those with a suitable middle digit).  It
   ignores indices < 1 (behaviour is undefined for n == 0). */

#include <stdbool.h>

/* Helper: reverse the decimal representation of an integer of type seq_t.
   The number must fit into seq_t; the routine works for any seq_t. */
static seq_t rev_uint(seq_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* a(1) is 0 as specified */
    if (n == 1)
        return 0;

    /* remaining indices after removing the first term */
    idx_t rem = n - 1;

    /* iterate over possible lengths */
    for (idx_t L = 2; ; ++L) {
        if ((L & 1) == 0) {            /* even length 2*k */
            idx_t k = L >> 1;          /* digits in left half */
            /* count of palindromes of this length */
            idx_t count = 9;
            for (idx_t i = 1; i < k; ++i)
                count *= 10;
            if (rem > count) {
                rem -= count;
                continue;
            }
            /* rem‑th palindrome of this length (1‑based) */
            idx_t idx = rem - 1;           /* 0‑based within this length */
            /* left half starts at 10^(k-1) */
            idx_t left_start = 1;
            for (idx_t i = 1; i < k; ++i)
                left_start *= 10;
            idx_t left = left_start + idx;

            /* build the palindrome: left followed by reverse(left) */
            seq_t val = left;
            idx_t tmp = left;
            for (idx_t i = 0; i < k; ++i) {
                val = val * 10 + (tmp % 10);
                tmp /= 10;
            }
            return val;
        } else {                        /* odd length 2*k+1 */
            idx_t k = (L >> 1);        /* left half length (k digits) */
            /* left half ranges from 10^(k-1) to 10^k-1 (inclusive) */
            idx_t left_start = 1;
            for (idx_t i = 1; i < k; ++i)
                left_start *= 10;
            idx_t left_end = left_start * 10 - 1;
            for (idx_t left = left_start; left <= left_end; ++left) {
                /* get the individual digits of the left half */
                int digits[10];
                idx_t tmp = left;
                for (idx_t pos = k; pos > 0; --pos) {
                    digits[pos - 1] = tmp % 10;
                    tmp /= 10;
                }
                /* compute D_left = sum_even - sum_odd over the left half */
                int odd_sum = 0, even_sum = 0;
                for (idx_t j = 1; j <= k; ++j) {
                    if ((j & 1) == 0)
                        even_sum += digits[j - 1];
                    else
                        odd_sum += digits[j - 1];
                }
                int D_left = even_sum - odd_sum;

                /* middle position parity */
                int mid_digit;
                if (((k + 1) & 1) == 0)           /* middle even */
                    mid_digit = -2 * D_left;
                else                               /* middle odd */
                    mid_digit = 2 * D_left;

                if (mid_digit >= 0 && mid_digit <= 9) {
                    rem -= 1;
                    if (rem == 0) {
                        /* construct the palindrome: left, mid_digit, reverse(left) */
                        seq_t val = left;
                        val = val * 10 + mid_digit;
                        tmp = left;
                        for (idx_t i = 0; i < k; ++i) {
                            val = val * 10 + (tmp % 10);
                            tmp /= 10;
                        }
                        return val;
                    }
                }
            }
            /* if we reach here, the length was insufficient; continue */
        }
    }
}
