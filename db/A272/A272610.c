/*
 * A272610  a(1)=5, a(2)=9, a(3)=4, a(4)=6; thereafter a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)).
 * https://oeis.org/A272610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) with a(1)=5, a(2)=9, a(3)=4,
 *            a(4)=6 and a(n)=0 for n<=0
 * Range:     u8 n <= 166, u16 n <= 7536, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static memoisation tables */
static seq_t *cache = NULL;          /* stores a(n) for n>=1 */
static unsigned char *seen = NULL;   /* 1 if cache[n] is valid */
static idx_t cache_size = 0;         /* current allocated size (>=1) */

/* enlarge tables so that indices up to 'needed' (>=1) are valid */
static void grow_cache(idx_t needed)
{
    if (needed < cache_size) return;

    idx_t new_size = needed + 1;      /* include index 0 unused */
    if (new_size < 64) new_size = 64;

    seq_t *tmp_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    unsigned char *tmp_seen = (unsigned char *)realloc(seen,
                                                       new_size * sizeof(unsigned char));
    if (!tmp_cache || !tmp_seen) {
        /* Out of memory – terminate */
        exit(1);
    }
    cache = tmp_cache;
    seen  = tmp_seen;

    /* initialise newly allocated elements */
    for (idx_t i = cache_size; i < new_size; ++i) {
        cache[i] = (seq_t)0;
        seen[i]  = 0;
    }
    cache_size = new_size;
}

/* recursive worker with memoisation */
static seq_t compute_value(idx_t n)
{
    if (n == 0) return (seq_t)0;

    grow_cache(n);

    if (seen[n]) return cache[n];

    seq_t result;
    if (n <= 4) {
        switch (n) {
            case 1: result = (seq_t)5; break;
            case 2: result = (seq_t)9; break;
            case 3: result = (seq_t)4; break;
            case 4: result = (seq_t)6; break;
            default: result = (seq_t)0; /* unreachable */
        }
    } else {
        /* ensure a(n-1) and a(n-2) are computed */
        seq_t a_n1 = compute_value(n - 1);
        seq_t a_n2 = compute_value(n - 2);

        /* compute indices for recursive calls */
        idx_t idx1 = 0, idx2 = 0;
        if (n > (idx_t)a_n1) idx1 = n - (idx_t)a_n1;
        if (n > (idx_t)a_n2) idx2 = n - (idx_t)a_n2;

        seq_t v1 = compute_value(idx1);
        seq_t v2 = compute_value(idx2);

        result = v1 + v2;
    }

    cache[n] = result;
    seen[n]  = 1;
    return result;
}

/* public interface */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    return compute_value(n);
}
