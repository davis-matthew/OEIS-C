/*
 * A297825  a(n) is the (negative) length of the final string after
 *          iteratively removing all runs from the binary string 11011100...n
 *          (formed by concatenating the first n binary numbers, see
 *          A058935(n)).
 * https://oeis.org/A297825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For n ≥ 1 construct the binary concatenation
   of the integers 1…n.  Repeatedly delete every maximal run
   of two or more identical bits until the string has no such
   run.  The final string is empty or alternating.  The value
   a(n) is the (signed) length of that final string:
   positive if it starts with 1, negative if it starts with 0. */

/* The function is written in pure C (C11) and will also compile
   as C++17.  It does not use any C++ or external library
   features. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* capacity that the static buffers must have */
    static unsigned char *buf1 = NULL;
    static unsigned char *buf2 = NULL;
    static size_t buf_cap = 0;

    /* estimate the maximum number of bits needed for this n
       = n * (floor_log2(n)+1)  <  n * (floor_log2(n)+2)  for safety   */
    unsigned int max_bits_per = 0;
    idx_t temp = n;
    while (temp >>= 1) { ++max_bits_per; }
    /* max_bits_per = floor_log2(n) */
    size_t needed = (size_t)n * (max_bits_per + 2) + 16;

    if (buf_cap < needed) {
        free(buf1);
        free(buf2);
        buf_cap = needed;
        buf1 = (unsigned char *)malloc(buf_cap);
        buf2 = (unsigned char *)malloc(buf_cap);
    }

    unsigned char *s  = buf1;
    unsigned char *s2 = buf2;

    /* ---- build the binary concatenation of 1 .. n ---- */
    size_t len = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* find most significant bit of k */
        unsigned int msb = 0;
        idx_t t = k;
        while (t >>= 1) { ++msb; }
        /* output bits from msb down to 0 */
        for (int b = (int)msb; b >= 0; --b) {
            s[len++] = (unsigned char)((k >> b) & 1);
        }
    }

    /* ---- iteratively delete runs of length ≥ 2 ---- */
    while (1) {
        size_t len2 = 0;
        int changed = 0;
        size_t i = 0;
        while (i < len) {
            size_t j = i + 1;
            while (j < len && s[j] == s[i]) ++j;
            size_t runlen = j - i;
            if (runlen >= 2) {
                changed = 1;          /* this run will be removed */
            } else {
                s2[len2++] = s[i];    /* keep single bit */
            }
            i = j;
        }
        if (!changed) break;           /* no more deletions */
        memcpy(s, s2, len2);
        len = len2;
    }

    /* ---- interpret the final string ---- */
    if (len == 0) {
        return (seq_t)0;
    } else {
        seq_t result = (seq_t)len;
        return (s[0] == 1) ? result : -result;
    }
}
