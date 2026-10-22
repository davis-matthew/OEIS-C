/*
 * A335649  a(n) is the frequency of multi-pairs in a sequence of multi-set
 *          designs with 2 blocks.
 * https://oeis.org/A335649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 76*a(n-2) + 76*a(n-3) - 19*a(n-4) + a(n-5);
 *            Initial values: a(1)=0, a(2)=10, a(3)=200, a(4)=3040, a(5)=43320
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache holding the pre‑computed terms
   cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache = NULL;
static size_t count = 0;   /* number of terms currently stored   */
static size_t capacity = 0;/* allocated capacity                 */

/* ensure the cache can hold at least `need` terms */
static void ensure_cache_capacity(idx_t need)
{
    if (capacity >= (size_t)need)
        return;
    size_t new_cap = capacity ? capacity * 2 : 5;
    while (new_cap < (size_t)need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    capacity = new_cap;
}

/* initialise the cache with the first 5 terms */
static void init_cache(void)
{
    if (count != 0)
        return;
    ensure_cache_capacity(5);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)10;
    cache[2] = (seq_t)200;
    cache[3] = (seq_t)3040;
    cache[4] = (seq_t)43320;
    count = 5;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;

    init_cache();       /* guarantees the first 5 terms are stored */

    if ((size_t)n <= count)
        return cache[n-1];   /* already computed */

    ensure_cache_capacity((size_t)n);

    /* compute terms a(count+1) ... a(n) */
    for (size_t i = count; i < (size_t)n; ++i) {
        /* i corresponds to index n (1‑based) */
        seq_t a1 = cache[i-1];   /* a(i)  = a(i+1 -1)  */
        seq_t a2 = cache[i-2];   /* a(i-1) = a(i+1 -2)*/
        seq_t a3 = cache[i-3];   /* a(i-2) */
        seq_t a4 = cache[i-4];   /* a(i-3) */
        seq_t a5 = cache[i-5];   /* a(i-4) */

        seq_t term = (seq_t)19 * a1
                   - (seq_t)76 * a2
                   + (seq_t)76 * a3
                   - (seq_t)19 * a4
                   +          a5;
        cache[i] = term;
    }

    count = (size_t)n;
    return cache[n-1];
}
