/*
 * A336529  a(n) = (n^3+5*n+3)/3 + 2*floor(n/2) + a(n-2), with a(0)=1 and
 *          a(1)=3.
 * https://oeis.org/A336529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3+5*n+3)/3 + 2*floor(n/2) + a(n-2), a(0)=1, a(1)=3
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 565, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static idx_t   seq_cache_max = (idx_t)-1;

/* Compute the n-th term of A336529 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 3;

    /* Allocate or grow cache if necessary */
    if (seq_cache == NULL) {
        seq_cache_cap = (n + 1) > 4 ? (n + 1) : 4;
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
        if (!seq_cache) return 0;          /* memory allocation failed */
        seq_cache[0] = 1;
        seq_cache[1] = 3;
        seq_cache_max = 1;
    } else if ((size_t)(n + 1) > seq_cache_cap) {
        size_t new_cap = seq_cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t term = (i_seq * i_seq * i_seq + 5 * i_seq + 3) / 3;
        term += 2 * (i / 2);
        term += seq_cache[i - 2];
        seq_cache[i] = term;
    }
    seq_cache_max = n;
    return seq_cache[n];
}
