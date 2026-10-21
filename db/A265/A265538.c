/*
 * A265538  Largest base-7 palindrome m <= n, written in base 7.
 * https://oeis.org/A265538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest base‑7 palindrome m ≤ n, with its base‑7 digits
 *            interpreted as a decimal integer
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑7 digits (most significant first) */
    idx_t temp = n;
    int k = 0;
    idx_t digits[64];
    if (temp == 0) {
        digits[0] = 0;
        k = 1;
    } else {
        while (temp > 0) {
            digits[k++] = temp % (idx_t)7;
            temp /= (idx_t)7;
        }
        /* reverse to most significant first */
        for (int i = 0; i < k / 2; ++i) {
            idx_t t = digits[i];
            digits[i] = digits[k - 1 - i];
            digits[k - 1 - i] = t;
        }
    }

    /* Construct the palindrome candidate from the left half */
    int lenL = (k + 1) / 2;          /* length of left part (including middle if odd) */
    idx_t left[64];
    idx_t pdigits[64];
    for (int i = 0; i < lenL; ++i) left[i] = digits[i];
    for (int i = 0; i < k; ++i) {
        if (i < lenL) pdigits[i] = left[i];
        else pdigits[i] = left[k - 1 - i];
    }

    /* Compute numeric value of the candidate in base‑10 (numeric value) */
    idx_t val_p = 0;
    for (int i = 0; i < k; ++i) val_p = val_p * (idx_t)7 + pdigits[i];

    if (val_p <= n) {
        /* Candidate works – build decimal representation */
        seq_t ans = 0;
        for (int i = 0; i < k; ++i) ans = ans * (seq_t)10 + (seq_t)pdigits[i];
        return ans;
    }

    /* Need to decrement the left half */
    int i;
    for (i = lenL - 1; i >= 0; --i) {
        if (left[i] > 0) { left[i]--; break; }
        left[i] = 6;
    }

    if (left[0] == 0) {
        /* Leading zero → use the largest palindrome of length k‑1 (all 6s) */
        int new_k = k - 1;
        seq_t ans = 0;
        for (int j = 0; j < new_k; ++j) ans = ans * (seq_t)10 + (seq_t)6;
        return ans;
    }

    /* Build palindrome of same length using decremented left half */
    idx_t ndigits[64];
    for (int j = 0; j < k; ++j) {
        if (j < lenL) ndigits[j] = left[j];
        else ndigits[j] = left[k - 1 - j];
    }

    seq_t ans = 0;
    for (int j = 0; j < k; ++j) ans = ans * (seq_t)10 + (seq_t)ndigits[j];
    return ans;
}
