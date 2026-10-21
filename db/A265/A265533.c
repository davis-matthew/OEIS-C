/*
 * A265533  Largest base-5 palindrome m <= n, written in base 10.
 * https://oeis.org/A265533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest base‑5 palindrome ≤ n, value returned in decimal (base 10). */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Convert n to base‑5 digits (most significant digit first). */
    seq_t temp = (seq_t)n;
    unsigned char rev[256];
    size_t len = 0;
    while (temp > 0) {
        rev[len++] = (unsigned char)(temp % 5);
        temp /= 5;
    }
    unsigned char digits[256];
    for (size_t i = 0; i < len; ++i) digits[i] = rev[len - 1 - i];

    /* Build the largest palindrome not exceeding n. */
    unsigned char pal[256];
    for (size_t i = 0; i < len; ++i) {
        if (i < len / 2) pal[i] = digits[i];
        else pal[i] = digits[len - 1 - i];
    }

    /* Compute its decimal value. */
    seq_t candidate = 0;
    for (size_t i = 0; i < len; ++i) candidate = candidate * 5 + pal[i];
    seq_t n_orig = (seq_t)n;

    if (candidate <= n_orig) return candidate;

    /* Decrement the left (and middle, if exists) part of the palindrome. */
    size_t prefix_len = (len % 2 == 0) ? len / 2 : (len + 1) / 2;
    unsigned char prefix[256];
    for (size_t i = 0; i < prefix_len; ++i) prefix[i] = digits[i];

    int i = (int)prefix_len - 1;
    while (i >= 0) {
        if (prefix[i] > 0) { prefix[i]--; break; }
        prefix[i] = 4;      /* borrow from the next digit to the left */
        --i;
    }

    /* If the leading digit became zero, the palindrome length reduces. */
    if (i < 0 || prefix[0] == 0) {
        if (len <= 1) return 0;   /* happens only when n = 0, handled earlier */
        seq_t all4 = 0;
        for (size_t k = 0; k + 1 < len; ++k) all4 = all4 * 5 + 4;
        return all4;
    }

    /* Construct the new palindrome from the decremented prefix. */
    for (size_t j = 0; j < prefix_len; ++j) pal[j] = prefix[j];
    for (size_t j = prefix_len; j < len; ++j) pal[j] = prefix[len - 1 - j];

    seq_t new_candidate = 0;
    for (size_t j = 0; j < len; ++j) new_candidate = new_candidate * 5 + pal[j];
    return new_candidate;
}
