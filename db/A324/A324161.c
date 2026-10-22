/*
 * A324161  Number of zerofree nonnegative integers <= n.
 * https://oeis.org/A324161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of integers 0 ≤ x ≤ n whose decimal representation
 *            contains no '0' digit
 * Range:     u8 n <= 313, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple digit‑DP that runs in O(number of decimal digits). */

#include <stddef.h>   /* for size_t */

static seq_t pow9_cache[65];   /* cache 9^k up to 64 digits (enough for uint64_t input) */

/* Count zerofree numbers up to n (inclusive). */
seq_t a(idx_t n)
{
    /* 0 is not counted because it contains the digit 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Decompose n into decimal digits (most significant first). */
    idx_t tmp = n;
    int digits_rev[64];   /* reversed digits */
    int pos = 0;
    while (tmp > (idx_t)0)
    {
        digits_rev[pos++] = (int)(tmp % (idx_t)10);
        tmp /= (idx_t)10;
    }
    int len = pos;               /* number of digits */
    int digits[64];
    for (int i = 0; i < len; ++i)
        digits[i] = digits_rev[len - 1 - i];

    /* Precompute powers of 9 up to len. */
    pow9_cache[0] = (seq_t)1;
    for (int i = 1; i <= len; ++i)
        pow9_cache[i] = pow9_cache[i - 1] * (seq_t)9;

    seq_t result = (seq_t)0;

    /* Count all numbers with fewer digits: 9^1 + 9^2 + ... + 9^(len-1). */
    for (int l = 1; l < len; ++l)
        result += pow9_cache[l];

    /* Count numbers with the same length ≤ n. */
    for (int i = 0; i < len; ++i)
    {
        int d = digits[i];
        if (d == 0)          /* a '0' in n stops further valid prefixes */
            break;

        int smaller_choices = d - 1;          /* digits 1..d-1 are allowed */
        if (smaller_choices > 0)
        {
            int remaining = len - i - 1;
            result += (seq_t)smaller_choices * pow9_cache[remaining];
        }
        /* If d == 0 we would have broken already. */
    }

    /* If n itself has no zero digit, include it. */
    int has_zero = 0;
    for (int i = 0; i < len; ++i)
    {
        if (digits[i] == 0)
        {
            has_zero = 1;
            break;
        }
    }
    if (!has_zero)
        result += (seq_t)1;

    return result;
}
