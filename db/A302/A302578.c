/*
 * A302578  Numbers N such that N modulo N's digitsum is a substring of N.
 * https://oeis.org/A302578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer N (n≥1) such that N %
 *            digit_sum(N) is a substring of N; a(1) = 10
 * Range:     u8 n <= 116, u16 n <= 8307, u32 n <= 8307, u64 n <= 8307,
 *            u128 n <= 8307, big any n
 * Helpers:   digit_sum, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_to_str.h"

/* Return true iff N has the required property. */
static bool has_property(seq_t N)
{
    seq_t ds = digit_sum(N);
    if (ds == (seq_t)0) {
        return false;
    }
    seq_t r = N % ds;

    /* convert N and r to decimal strings */
    char nbuf[128];
    char rbuf[128];
    size_t nlen, rlen;

    seq_to_str(N, nbuf, &nlen);
    nbuf[nlen] = '\0';          /* ensure null terminator */
    seq_to_str(r, rbuf, &rlen);
    rbuf[rlen] = '\0';

    return strstr(nbuf, rbuf) != NULL;
}

/* Cache of sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t next_candidate = 10;   /* first term is 10 */

/* Ensure the cache can hold at least needed elements. */
static void ensure_cache_size(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap == 0 ? 64 : seq_cap << 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap    = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0; return 0. */
        return (seq_t)0;
    }
    /* Find terms until we have at least n elements. */
    while (seq_len < (size_t)n) {
        if (has_property(next_candidate)) {
            ensure_cache_size(seq_len + 1);
            seq_cache[seq_len++] = next_candidate;
        }
        next_candidate++;
    }
    return seq_cache[n - 1];
}
