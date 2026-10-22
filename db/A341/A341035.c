/*
 * A341035  a(n) is the smallest positive integer such that n+a(n) contains
 *          the string n-a(n), in both forward and reverse directions, as a
 *          substring. If no such number exists then a(n) = -1.
 * https://oeis.org/A341035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 67, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341035: a(n) is the smallest positive integer k such that
   n+k contains n-k and reverse(n-k) as decimal substrings.
   If no such k exists, a(n) = -1.  For n <= 4 the answer is -1. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/get_digits.h"

/* Convert a seq_t to a nul‑terminated decimal string.
   The caller must provide a buffer big enough for the result. */
static size_t seqt_to_str(seq_t value, char *buf, size_t maxlen)
{
    uint8_t d[128];
    size_t dlen = 0;
    get_digits(value, d, &dlen);          /* digits in little‑endian order */

    /* Build the string in normal order (most significant first). */
    size_t i, j;
    for (i = 0, j = dlen; j-- > 0; ++i)
    {
        if (i + 1 >= maxlen) break;       /* safety: prevent overflow */
        buf[i] = '0' + d[j];
    }
    if (i + 1 < maxlen) buf[i] = '\0';
    return i;
}

/* Naïve substring search.  Returns true if needle occurs in hay. */
static bool contains(const char *hay, const char *needle)
{
    if (!*needle) return true;              /* empty needle matches everywhere */

    const char *p = hay;
    while (*p)
    {
        if (*p == *needle)
        {
            const char *h = p, *n = needle;
            while (*h && *n && *h == *n) { ++h; ++n; }
            if (!*n) return true;
        }
        ++p;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;

    /* Buffer for string conversions: enough for all seqₜ values we may
       encounter for n ≤ 10000 and corresponding a(n).  128 bytes is safe
       for 128‑bit integers.  For arbitrary‑precision types the build
       provides a bigger buffer internally. */
    char sum_buf[128];
    char diff_buf[64];
    char rev_buf[64];

    for (seq_t k = 1; k <= n_val; ++k)
    {
        seq_t sum = n_val + k;
        seq_t diff = n_val - k;
        if (diff < 0) continue;

        size_t sum_len = seqt_to_str(sum, sum_buf, sizeof(sum_buf));
        size_t diff_len = seqt_to_str(diff, diff_buf, sizeof(diff_buf));

        /* Build reverse of diff_buf. */
        for (size_t i = 0; i < diff_len; ++i)
            rev_buf[i] = diff_buf[diff_len - 1 - i];
        rev_buf[diff_len] = '\0';

        if (contains(sum_buf, diff_buf) &&
            contains(sum_buf, rev_buf))
            return k;
    }
    return -1;
}
