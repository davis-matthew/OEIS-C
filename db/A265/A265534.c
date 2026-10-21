/*
 * A265534  Largest base-5 palindrome m <= n, written in base 5.
 * https://oeis.org/A265534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = largest base‑5 palindrome <= n,
   returned as the base‑5 digits written in decimal. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 is a palindrome of length 1. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Convert n to base‑5 digits (most significant first). */
    idx_t tmp = n;
    int digits[64];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 5;
        tmp /= 5;
    }
    /* Reverse to most significant first. */
    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* The prefix that will be mirrored. */
    int k = (len + 1) / 2;            /* length of the prefix */
    int prefix[64];
    for (int i = 0; i < k; ++i) {
        prefix[i] = digits[i];
    }

    /* Main loop: construct palindrome, compare, decrement prefix if needed. */
    while (1) {
        /* Build palindrome for the current prefix. */
        int pal[64];
        for (int i = 0; i < len; ++i) {
            pal[i] = (i < k) ? prefix[i] : prefix[len - 1 - i];
        }

        /* Compare palindrome with the original number. */
        int cmp = 0; /* -1 if pal < n, 0 if equal, 1 if pal > n */
        for (int i = 0; i < len; ++i) {
            if (pal[i] < digits[i]) { cmp = -1; break; }
            if (pal[i] > digits[i]) { cmp =  1; break; }
        }

        if (cmp <= 0) {
            /* Palindrome is <= n; compute its value as decimal digits. */
            seq_t val = 0;
            for (int i = 0; i < len; ++i) {
                val = val * 10 + (seq_t)pal[i];
            }
            return val;
        }

        /* Palindrome > n: decrement the prefix (base‑5). */
        int idx = k - 1;
        while (idx >= 0) {
            if (prefix[idx] > 0) {
                prefix[idx]--;
                break;
            }
            prefix[idx] = 4;   /* borrow */
            idx--;
        }

        /* If we borrowed past the most significant digit or the leading digit
           became 0, the largest palindrome is all 4's of length (len-1). */
        if (idx < 0 || prefix[0] == 0) {
            int len1 = len - 1;
            if (len1 == 0) {
                return (seq_t)0;
            }
            seq_t val = 0;
            for (int i = 0; i < len1; ++i) {
                val = val * 10 + (seq_t)4;
            }
            return val;
        }
    }
}
