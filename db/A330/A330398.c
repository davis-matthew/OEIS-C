/*
 * A330398  For any n > 0, let S_n(0) be the concatenation of the numbers in
 *          the range 1..n (in increasing order); for k=1..n, S_n(k) is
 *          obtained by removing the rightmost occurrence of k in S_n(k-1), if
 *          any; a(n) is the numerical value of S_n(n) (or 0 if S_n(n) is
 *          empty).
 * https://oeis.org/A330398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 17, u64 n <= 107,
 *            u128 n <= 117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A330398: concatenate 1..n, then
   for k = 1..n delete the rightmost occurence of the decimal
   representation of k.  a(n) is the resulting number, or 0 if the
   string becomes empty. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Helper: number of decimal digits of a positive uint64_t.  */
static size_t digit_count_uint64(uint64_t v) {
    size_t c = 0;
    do { ++c; v /= 10; } while (v);
    return c;
}

/* Return the number of digits of x (x > 0).  */
static size_t digit_len_seq(seq_t x) {
    /* x fits into seq_t; it may be large, so use built‑in conversion
       for small values.  We expect the value to fit in a long long
       when this is called for counting digits of intermediate strings. */
    if (x == 0) return 1;
    size_t len = 0;
    while (x != 0) {
        ++len;
        x /= (seq_t)10;
    }
    return len;
}

/* Main sequence function.  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* offset 1, but safe for n==0 */

    /* Compute the total length of the initial concatenation. */
    size_t total_len = 0;
    for (idx_t i = 1; i <= n; ++i) {
        total_len += digit_count_uint64((uint64_t)i);
    }

    /* Allocate buffer for the concatenated string. */
    char *S = (char *)malloc(total_len);
    if (!S) return (seq_t)0;   /* allocation failure */

    /* Build the initial string S_0. */
    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t t = (uint64_t)i;
        char tmp[20];
        int len = 0;
        /* Convert to decimal in reverse order. */
        do {
            tmp[len++] = '0' + (t % 10);
            t /= 10;
        } while (t);
        /* Append in normal order. */
        for (int j = len - 1; j >= 0; --j) {
            S[pos++] = tmp[j];
        }
    }

    /* Successively delete the rightmost occurrence of each k. */
    size_t cur_len = total_len;
    for (idx_t k = 1; k <= n; ++k) {
        /* Build decimal representation of k. */
        uint64_t t = (uint64_t)k;
        char dtmp[20];
        int dlen = 0;
        do {
            dtmp[dlen++] = '0' + (t % 10);
            t /= 10;
        } while (t);
        /* Reverse to normal order. */
        for (int a = 0, b = dlen - 1; a < b; ++a, --b) {
            char tmpc = dtmp[a];
            dtmp[a] = dtmp[b];
            dtmp[b] = tmpc;
        }

        /* Search for the rightmost occurrence. */
        bool found = false;
        for (long long i = (long long)cur_len - dlen; i >= 0; --i) {
            size_t j;
            for (j = 0; j < (size_t)dlen; ++j)
                if (S[i + j] != dtmp[j]) break;
            if (j == (size_t)dlen) {
                /* Match found at position i.  Remove it. */
                size_t start = (size_t)i;
                size_t tail = cur_len - (start + (size_t)dlen);
                if (tail > 0) {
                    memmove(&S[start], &S[start + (size_t)dlen], tail);
                }
                cur_len -= (size_t)dlen;
                found = true;
                break;
            }
        }
        /* If not found, nothing to remove for this k. */
        (void)found;
    }

    /* Convert remaining string to seq_t value. */
    seq_t value = (seq_t)0;
    for (size_t i = 0; i < cur_len; ++i) {
        value = value * (seq_t)10 + (seq_t)(S[i] - '0');
    }

    free(S);
    return value;
}
