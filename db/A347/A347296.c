/*
 * A347296  a(1) = 1; for n >= 1, if a(n) is even then a(n+1) = a(n) / 2,
 *          otherwise, say a(n) is the k-th odd term in the sequence, a(n+1) =
 *          a(n) + a(k).
 * https://oeis.org/A347296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 387, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n≥1, if a(n) even then a(n+1)=a(n)/2,
   else let k be the rank of a(n) among odd terms in the sequence,
   then a(n+1)=a(n)+a(k).  Indexing starts at 1. */
#include <stdlib.h>
#include <stdint.h>

/* static cache state */
static seq_t *seq_cache     = NULL;   /* cache array, 0..i used, index 0 unused */
static size_t seq_cache_cap = 0;     /* allocated capacity */
static idx_t   seq_cache_max = 0;    /* highest index computed so far */
static idx_t   odd_cnt      = 0;     /* number of odd terms seen so far */

/* ensure capacity large enough for index n */
static void grow_cache(idx_t n)
{
    if (seq_cache_cap <= n) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap : 2;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();  /* allocation failure */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
}

/* make sure all terms up to index n are computed */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {
        /* first call – initialise for a(1) */
        seq_cache_cap = 2;
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
        if (seq_cache == NULL) abort();
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)1;
        odd_cnt     = 1;        /* a(1) is odd */
        seq_cache_max = 1;
    }

    if (seq_cache_max >= n)
        return;                 /* already have everything needed */

    grow_cache(n);

    for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
        seq_t cur  = seq_cache[(size_t)(i - 1)];
        seq_t next;
        if ((cur & 1) == 0) {
            next = cur / 2;
        } else {
            /* cur is odd; its rank among odd terms is odd_cnt */
            next = cur + seq_cache[(size_t)odd_cnt];
        }
        seq_cache[(size_t)i] = next;
        if ((next & 1) != 0)
            odd_cnt++;
    }
    seq_cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, guard */
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
