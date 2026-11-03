/*
 * A261676  Numbers which when represented as a sum of palindromes using the
 *          greedy algorithm require more than 3 palindromes.
 * https://oeis.org/A261676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261676: numbers n such that the greedy algorithm for writing
 * n as a sum of decimal palindromes uses more than three palindromes. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t largest_palindrome_leq(seq_t x)
{
    if (x < 10) return x;

    const int len = num_digits(x);
    const int half_len = (len + 1) / 2;
    const seq_t pow10v = pow10(len - half_len);
    seq_t firstHalf = x / pow10v;

    /* Build the palindrome from firstHalf */
    seq_t pal = firstHalf;
    seq_t tmp = firstHalf;
    if (len % 2) tmp /= 10;
    while (tmp) {
        pal = pal * 10 + (tmp % 10);
        tmp /= 10;
    }

    if (pal > x) {
        /* Decrease the first half */
        firstHalf -= 1;
        /* If it fell to zero, the largest palindrome is all 9's of length len-1 */
        if (firstHalf == 0) return pow10(len - 1) - 1;
        /* If the shortened firstHalf has fewer digits, also return all 9's */
        if (num_digits(firstHalf) < half_len) return pow10(len - 1) - 1;

        /* Rebuild the palindrome with the decremented firstHalf */
        pal = firstHalf;
        tmp = firstHalf;
        if (len % 2) tmp /= 10;
        while (tmp) {
            pal = pal * 10 + (tmp % 10);
            tmp /= 10;
        }
    }

    return pal;
}

static int greedy_len(seq_t n)
{
    int cnt = 0;
    while (n > 0) {
        seq_t p = largest_palindrome_leq(n);
        n -= p;
        cnt++;
        if (cnt > 3) break;          /* early exit for speed */
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 1;

    while (found < n) {
        if (greedy_len(x) > 3) {
            ++found;
            if (found == n) return x;
        }
        ++x;
    }
    return 0;   /* unreachable, keeps compiler happy */
}
