/*
 * A378934  Number of minimal edge cuts in the 4 X n grid graph.
 * https://oeis.org/A378934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=28, a(3)=146, a(4)=627, a(5)=2471, * a(6)=9292,
 *            a(7)=33878, a(8)=120771, a(9)=423251 * Recurrence for n≥10: *
 *            a(n) = 10*a(n-1) - 37*a(n-2) + 59*a(n-3) - 27*a(n-4) * -
 *            25*a(n-5) + 21*a(n-6) + 5*a(n-7) - 4*a(n-8) - a(n-9)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Initial terms */
static const seq_t init_vals[9] = {
    (seq_t)3ULL,
    (seq_t)28ULL,
    (seq_t)146ULL,
    (seq_t)627ULL,
    (seq_t)2471ULL,
    (seq_t)9292ULL,
    (seq_t)33878ULL,
    (seq_t)120771ULL,
    (seq_t)423251ULL
};

/* Cached values of the sequence */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;   /* number of computed terms */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure cache can hold terms up to n (1‑based index) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;

    size_t need = (size_t)n;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort();  /* allocation failure */
    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;    /* undefined for n < 1, return 0 */

    /* Return initial values directly if requested */
    if (n <= (idx_t)9)
        return init_vals[(size_t)(n - 1)];

    /* First time: initialise cache with the 9 known terms */
    if (cache_size == 0) {
        ensure_cache(9);
        for (size_t i = 0; i < 9; ++i)
            seq_cache[i] = init_vals[i];
        cache_size = 9;
    }

    /* Extend cache if necessary */
    if ((size_t)n > cache_size)
        ensure_cache(n);

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        /* i is 1‑based index */
        seq_t term = seq_cache[i - 2] * (seq_t)10;   /* a(i-1)   */
        term -= seq_cache[i - 3] * (seq_t)37;       /* a(i-2)   */
        term += seq_cache[i - 4] * (seq_t)59;       /* a(i-3)   */
        term -= seq_cache[i - 5] * (seq_t)27;       /* a(i-4)   */
        term -= seq_cache[i - 6] * (seq_t)25;       /* a(i-5)   */
        term += seq_cache[i - 7] * (seq_t)21;       /* a(i-6)   */
        term += seq_cache[i - 8] * (seq_t)5;        /* a(i-7)   */
        term -= seq_cache[i - 9] * (seq_t)4;        /* a(i-8)   */
        term -= seq_cache[i -10];                  /* a(i-9)   */

        seq_cache[i - 1] = term;
    }

    cache_size = (size_t)n;
    return seq_cache[(size_t)n - 1];
}
