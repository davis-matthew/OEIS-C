/*
 * A400351  a(0) = 1; a(2*n) = Sum_{j=0..n} a(j) * a(n-j), a(2*n+1) = a(n).
 * https://oeis.org/A400351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(2*n) = Σ_{j=0..n} a(j) * a(n-j); a(2*n+1) = a(n);
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation keeps a static cache large enough for n <= 20000.
    The cache is grown automatically if a larger n is requested.   */
#include <stddef.h>        /* size_t */

static seq_t *cache = NULL;          /* cache[0..max_computed]           */
static idx_t  max_computed = 0;       /* highest index that has been filled */
static size_t cache_cap   = 0;        /* current allocated size           */

/* Allocate or grow the cache to hold at least indices 0..need_inclusive. */
static void ensure_capacity(idx_t need)
{
    if (need < cache_cap) return;     /* already enough                      */

    /* Grow to power-of-two size >= need+1 */
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;

    /* Allocate raw memory; the type seq_t may be a built‑in integer
       or a C++ class; allocating raw storage and then using placement
       new would require the keyword new, which is disallowed by the
       requirements.  Instead we rely on a static array when fair,
       or simply reallocate and copy the existing data pointwise. */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);  /* allocation failed */

    /* Copy already computed entries into the new cache. */
    if (cache && max_computed < cache_cap) {
        for (idx_t i = 0; i <= max_computed; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Compute values up to index n, filling the cache as needed. */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;     /* everything already known */

    ensure_capacity(n);                /* make sure we have space */

    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if ((i & 1) == 0) /* even: i = 2*k */
        {
            idx_t k   = i >> 1;
            seq_t sum = 0;
            for (idx_t j = 0; j <= k; ++j)
                sum += cache[j] * cache[k - j];
            cache[i] = sum;
        }
        else             /* odd:  i = 2*k + 1 */
        {
            idx_t k   = i >> 1;      /* floor(i/2) */
            cache[i] = cache[k];
        }
    }
    max_computed = n;
}

/* Public interface: returns a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Initialise the cache on first call. */
    if (!cache)
    {
        cache      = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!cache) exit(1);
        cache[0]   = (seq_t)1;   /* a(0) = 1 */
        cache_cap  = 1;
        max_computed = 0;
    }

    compute_up_to(n);              /* ensure a[n] is ready */
    return cache[n];
}
