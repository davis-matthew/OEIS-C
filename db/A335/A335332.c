/*
 * A335332  Decimal representation of n-th iteration of the one-dimensional
 *          cellular automaton defined by Rule 954, based on the 4-celled von
 *          Neumann neighborhood starting with a single black cell.
 * https://oeis.org/A335332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A335332: Decimal representation of the n‑th iteration of the
   one‑dimensional cellular automaton defined by Rule 954,
   based on the 4‑celled von Neumann neighbourhood.
   Recurrence (n > 9):
       a(n) = 8*a(n-1) + 4*a(n-2) - 32*a(n-3) + a(n-4)
             - 8*a(n-5) - 4*a(n-6) + 32*a(n-7)
   Initial values:
       a(1)=1,  a(2)=11,  a(3)=81,  a(4)=699,
       a(5)=5441, a(6)=43723, a(7)=349265,
       a(8)=2797243, a(9)=22368577. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cache[0] is unused */
static idx_t  cache_size = 0;        /* allocated size (# of elements) */
static idx_t  cached_upto = 0;       /* highest index already computed */

seq_t a(idx_t n)
{
    /* --- initial terms (1‑based) --------------------------------- */
    static const seq_t init[10] = {
        (seq_t)0,  /* dummy for index 0 */
        (seq_t)1,  (seq_t)11,  (seq_t)81,
        (seq_t)699, (seq_t)5441, (seq_t)43723,
        (seq_t)349265, (seq_t)2797243, (seq_t)22368577
    };

    if (n == 0) return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* --- ensure cache has room for indices 1 … n ----------------- */
    if (cache_size <= n) {
        idx_t new_size = n + 1;              /* +1 because index 0 unused */
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
        /* copy existing values */
        for (idx_t i = 1; i <= cached_upto; ++i)
            new_cache[i] = cache[i];
        /* free old cache */
        delete[] cache;
#else
        new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 1; i <= cached_upto; ++i)
            new_cache[i] = cache[i];
        free(cache);
#endif
        cache = new_cache;
        cache_size = new_size;
    }

    /* --- compute terms up to n (if not already cached) ------------- */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i];
        } else {
            cache[i] =   (seq_t)8  * cache[i-1]
                       + (seq_t)4  * cache[i-2]
                       - (seq_t)32 * cache[i-3]
                       + cache[i-4]
                       - (seq_t)8  * cache[i-5]
                       - (seq_t)4  * cache[i-6]
                       + (seq_t)32 * cache[i-7];
        }
    }
    cached_upto = n;

    return cache[n];
}
