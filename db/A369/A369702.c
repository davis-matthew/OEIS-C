/*
 * A369702  Smallest k > n such that the decimal expansion of k*n contains n
 *          as a substring of consecutive decimal digits.
 * https://oeis.org/A369702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest k > n such that the decimal expansion of k*n
 *            contains the decimal expansion of n as a consecutive substring
 * Range:     u8 n <= 10, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/seq_to_str.h"   /* seq_to_str(seq_t x, char *buf, size_t *len) */
#include "../utils/digit_len.h"    /* digit_len(seq_t x) returns number of decimal digits */

/* Search for the first occurrence of pat[0..pat_len-1] inside big[0..big_len-1]. */
static bool contains_substring(const char *big, size_t big_len,
                               const char *pat, size_t pat_len)
{
    if (pat_len == 0 || pat_len > big_len) return false;
    for (size_t i = 0; i + pat_len <= big_len; ++i) {
        if (memcmp(big + i, pat, pat_len) == 0) return true;
    }
    return false;
}

/*  seq_t a(idx_t n)  ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Offset from the OEIS sequence: a(1) is the first term. */
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* Convert n to decimal string once. */
    seq_t n_seq = (seq_t)n;
    size_t n_len  = digit_len(n_seq);
    char *n_buf = (char *)malloc(n_len + 1);
    if (!n_buf) abort();
    seq_to_str(n_seq, n_buf, &n_len);
    n_buf[n_len] = '\0';

    /* Allocate a buffer large enough for any product that will be tested. */
    /* We grow it dynamically as needed. */
    char *prod_buf = NULL;
    size_t prod_cap = 0;

    seq_t k = (seq_t)(n + 1);
    while (1) {
        seq_t prod = n_seq * k;

        size_t prod_len = digit_len(prod);
        size_t needed = prod_len + 1;   /* +1 for terminating NUL */
        if (needed > prod_cap) {
            prod_buf = (char *)realloc(prod_buf, needed);
            if (!prod_buf) abort();
            prod_cap = needed;
        }
        seq_to_str(prod, prod_buf, &prod_len);
        prod_buf[prod_len] = '\0';

        if (contains_substring(prod_buf, prod_len, n_buf, n_len))
            break;

        ++k;
    }

    free(n_buf);
    free(prod_buf);
    return k;
}
