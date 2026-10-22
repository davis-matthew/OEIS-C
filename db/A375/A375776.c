/*
 * A375776  Bitwise conflict-free sequence: Each number n is placed into the
 *          first set k that contains no element x where n AND x > 0: a(n) =
 *          k.
 * https://oeis.org/A375776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = smallest k such that for all i < n with a(i)=k, n & i = 0
 * Range:     u8 n <= 404, u16 n <= 9335, u32 n <= 9335, u64 n <= 9335,
 *            u128 n <= 9335, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a growing cache of computed values and a
   list of bit masks for each color.  It works for all C11 and C++17
   builds, handling both built‑in integer types and arbitrary‑precision
   seq_t classes. */
#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
# include <new>
# include <cstring>
#endif

/*reallocate an array of seq_t, handling construction for C++ */
#ifdef __cplusplus
static seq_t* reallocate_seq(seq_t *old, size_t old_sz, size_t new_sz)
{
    seq_t *new_arr = static_cast<seq_t*>(::operator new[](new_sz * sizeof(seq_t)));
    for (size_t i = 0; i < old_sz; ++i)
        new(&new_arr[i]) seq_t(old[i]);          /* copy */
    for (size_t i = old_sz; i < new_sz; ++i)
        new(&new_arr[i]) seq_t();                /* default */
    ::operator delete[](old);                    /* destroy old */
    return new_arr;
}
#else
static seq_t* reallocate_seq(seq_t *old, size_t old_sz, size_t new_sz)
{
    seq_t *new_arr = (seq_t*)malloc(new_sz * sizeof(seq_t));
    if (!new_arr) return NULL;                    /* allocation failure */
    memset(new_arr, 0, new_sz * sizeof(seq_t));   /* zero init */
    memcpy(new_arr, old, old_sz * sizeof(seq_t));
    free(old);
    return new_arr;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 1 */

    /* static state */
    static seq_t *cache      = NULL;           /* a(1..cache_size)  */
    static size_t cache_size = 0;              /* allocated size    */
    static idx_t  max_computed = 0;            /* highest n computed*/

    static seq_t *used_mask   = NULL;           /* mask per color   */
    static size_t used_cap    = 0;              /* capacity */
    static size_t used_cnt    = 0;              /* number of colors */

    /* enlarge cache if needed */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *tmp = reallocate_seq(cache, cache_size, new_size);
        if (!tmp) return 0;                    /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* already computed? */
    if (cache[n - 1] != 0) return cache[n - 1];

    /* compute a(m) for m in (max_computed, n] */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t mask = (seq_t)m;
        size_t chosen = (size_t)-1;           /* sentinel for “none” */

        /* find first color with no bit overlap */
        for (size_t s = 0; s < used_cnt; ++s) {
            if ((used_mask[s] & mask) == 0) {
                chosen = s;
                break;
            }
        }

        if (chosen != (size_t)-1) {           /* reuse existing set */
            cache[m - 1] = (seq_t)(chosen + 1);
            used_mask[chosen] |= mask;
        } else {                              /* create a new set */
            if (used_cnt == used_cap) {
                size_t new_cap = used_cap ? used_cap << 1 : 16;
                seq_t *tmp = reallocate_seq(used_mask, used_cap, new_cap);
                if (!tmp) return 0;          /* allocation failure */
                used_mask = tmp;
                used_cap  = new_cap;
            }
            used_mask[used_cnt] = mask;
            cache[m - 1] = (seq_t)(used_cnt + 1);
            ++used_cnt;
        }
    }

    max_computed = n;
    return cache[n - 1];
}
