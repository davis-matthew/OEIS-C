/*
 * A250781  Number of (n+1) X (6+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the 9‑term linear recurrence a(n) = 10*a(n-1) –
 *            44*a(n-2) + 112*a(n-3) – 182*a(n-4) + 196*a(n-5) – 140*a(n-6) +
 *            64*a(n-7) – 17*a(n-8) + 2*a(n-9) for n ≥ 10, with the first nine
 *            terms listed below
 * Range:     u8 none, u16 n <= 7, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial values a(1) … a(9).  They are used as the seed for the recurrence. */
static const seq_t init_values[9] = {
    (seq_t)436, (seq_t)1106, (seq_t)2604, (seq_t)5882,
    (seq_t)12950, (seq_t)27986, (seq_t)59590, (seq_t)125334,
    (seq_t)260916
};

static seq_t *cache = NULL;     /* dynamic array holding computed terms   */
static size_t cache_cap = 0;    /* current capacity of cache (in terms)    */
static idx_t cached_len = 0;    /* number of terms that have already been computed   */

/* Expand the cache so that it holds at least 'size' terms.
   Any newly created entries are computed using the recurrence. */
static void expand_cache(idx_t size)
{
    if ((idx_t)cache_cap >= size) return;           /* nothing to do */

    if (cache == NULL) {
        /* First allocation: use malloc to avoid destructor problems.
           The objects are default‑constructed, which is fine for basic
           integer types and for the arbitrary‑precision type.  We never free
           the memory explicitly, so destructors are not a concern. */
        cache = (seq_t*)malloc(size * sizeof(seq_t));
        if (!cache) { exit(EXIT_FAILURE); }
    } else {
        /* Reallocate with increased capacity.  We cast the void* result
           to seq_t* because this is allowed in both C and C++. */
        seq_t *tmp = (seq_t*)realloc(cache, size * sizeof(seq_t));
        if (!tmp) { exit(EXIT_FAILURE); }
        cache = tmp;
    }

    /* Compute any terms that are missing. */
    for (size_t i = cached_len; i < (size_t)size; ++i) {
        if (i < 9) {
            /* Seed terms a(1) … a(9) */
            cache[i] = init_values[i];
        } else {
            /* Recurrence for a(i+1) (since i is 0‑based) */
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];
            seq_t a7 = cache[i-7];
            seq_t a8 = cache[i-8];
            seq_t a9 = cache[i-9];
            cache[i] = (seq_t)10 * a1
                       - (seq_t)44 * a2
                       + (seq_t)112 * a3
                       - (seq_t)182 * a4
                       + (seq_t)196 * a5
                       - (seq_t)140 * a6
                       + (seq_t)64 * a7
                       - (seq_t)17 * a8
                       + (seq_t)2 * a9;
        }
    }

    cache_cap = (size_t)size;
    cached_len = cache_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n=0 */

    /* Ensure the cache covers the requested index. */
    if (cache == NULL) {
        expand_cache((n > 9) ? n : 9);
    } else if ((idx_t)cache_cap < n) {
        expand_cache(n);
    }

    return cache[n-1];                          /* array is 0‑based */
}
