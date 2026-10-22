/*
 * A365873  The "triple commas" sequence, a variant of A121805. See the
 *          Comments and Example sections for detailed explanations.
 * https://oeis.org/A365873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 428, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the lexicographically earliest sequence for which, for every
   consecutive pair a(n-1), a(n), the difference a(n)-a(n-1) equals
   3 times the integer formed by the last digit of a(n-1) and the first
   digit of a(n). The sequence stops at a(634162)=99999961. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* number of terms stored, cache_len>0 iff cache[0]==1 */

/* Ensure cache can hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    /* realloc returns void*, cast is safe in both C and C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the next term after 'prev' */
static seq_t next_term(seq_t prev)
{
    const seq_t ten = 10;
    seq_t last_digit = prev % ten;          /* last digit of prev (0..9) */

    for (int f = 1; f <= 9; ++f) {
        seq_t f_seq = f;                    /* make f a seq_t */
        seq_t x = (seq_t)10 * last_digit + f_seq;
        seq_t delta = (seq_t)3 * x;
        seq_t cand = prev + delta;

        /* determine first digit of cand */
        seq_t t = cand;
        while (t >= ten)
            t /= ten;
        if (t == f_seq)
            return cand;
    }
    /* Should never be reached for this sequence */
    return prev + (seq_t)3 * ((seq_t)10 * last_digit + (seq_t)1);
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n<1, safe return */

    /* ensure the cache is large enough for n terms */
    grow_cache((size_t)n);

    /* initialise first term if necessary */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* generate missing terms */
    while (cache_len < (size_t)n) {
        seq_t next = next_term(cache[cache_len - 1]);
        cache[cache_len++] = next;
    }

    return cache[(size_t)n - 1];
}
