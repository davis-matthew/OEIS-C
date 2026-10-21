/*
 * A264478  Number of (4+1) X (n+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change 0,1 1,0 2,1 or -1,-1.
 * https://oeis.org/A264478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A264478
   Formula (recurrence for n > 11):
        a(n) =  9*a(n-2) + 54*a(n-3) + 60*a(n-4)
               -54*a(n-5) -279*a(n-6) -270*a(n-7)
               + 64*a(n-8) -864*a(n-9) -1728*a(n-10)
   Initial values (n = 1 … 11):
        1, 8, 16, 120, 456, 2272, 11044, 49200,
        236196, 1113296, 5157772
*/
#include <stdlib.h>

static seq_t *cache   = NULL; /* cache of computed terms */
static size_t cache_size = 0;  /* number of terms stored */
static size_t cache_cap  = 0;  /* allocated capacity */

/* initial “seed” values for n = 1 … 11 (zero‑based index 0 … 10) */
static const seq_t init_vals[11] = {
    (seq_t)1,
    (seq_t)8,
    (seq_t)16,
    (seq_t)120,
    (seq_t)456,
    (seq_t)2272,
    (seq_t)11044,
    (seq_t)49200,
    (seq_t)236196,
    (seq_t)1113296,
    (seq_t)5157772
};

/* Allocate or enlarge cache to hold at least ‘needed’ elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = (cache_cap > 1) ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    /* realloc returns void*, cast to seq_t* for both C and C++ */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* memory allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* sequence is 1‑indexed */

    const size_t idx = (size_t)n;    /* idx = n (zero‑based index is idx-1) */

    if (idx > cache_size) {
        ensure_capacity(idx);

        for (size_t i = cache_size; i < idx; ++i) {
            const size_t j = i + 1;   /* current n value (1‑based) */

            if (i < 11) {              /* seed values */
                cache[i] = init_vals[i];
            } else {
                /* recurrence for n > 11 (j = i+1 >= 12) */
                seq_t term = 0;

                term += cache[j - 3] * 9;    /* a(j-2) */
                term += cache[j - 4] * 54;   /* a(j-3) */
                term += cache[j - 5] * 60;   /* a(j-4) */
                term -= cache[j - 6] * 54;   /* a(j-5) */
                term -= cache[j - 7] * 279;  /* a(j-6) */
                term -= cache[j - 8] * 270;  /* a(j-7) */
                term += cache[j - 9] * 64;   /* a(j-8) */
                term -= cache[j -10] * 864;  /* a(j-9) */
                term -= cache[j -11] * 1728; /* a(j-10) */

                cache[i] = term;
            }
        }

        cache_size = idx;
    }

    return cache[idx - 1];
}
