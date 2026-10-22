/*
 * A365875  The "reversed commas" sequence, a variant of A121805. See the
 *          Comments and Example sections for detailed explanations.
 * https://oeis.org/A365875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini
 * Formula:   a(1) = 1. For n > 1, a(n) is the largest integer c > a(n‑1) such
 *            that rev( c - a(n‑1) ) = 10 * (last digit of a(n‑1)) + (first
 *            digit of c), where rev(x) is x written with its decimal digits
 *            reversed. The sequence ends when no such c exists
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

static seq_t *cache = NULL;        /* cached terms, 0‑based */
static idx_t cache_size = 0;       /* number of cached terms */
static size_t cache_cap = 0;       /* capacity of cache */

/* ensure cache has room for 'needed' terms (1‑based indexing) */
static void ensure_capacity(idx_t needed)
{
    if (needed <= (idx_t)cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while ((idx_t)new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise cache with a(1)=1 */
static void init_cache(void)
{
    if (cache) return;
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    cache[0] = (seq_t)1;   /* a(1) */
    cache_size = 1;
}

/* reverse decimal digits of a small integer (fits in 64 bits) */
static unsigned int rev_digits(unsigned int x)
{
    unsigned int r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* first decimal digit of a value */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10) x /= 10;
    return x;
}

/* compute the next term after 'prev' according to the rule */
static seq_t compute_next(seq_t prev)
{
    unsigned int dprev = (unsigned int)(prev % (seq_t)10);
    seq_t best = (seq_t)0;
    for (unsigned int f = 1; f <= 9; ++f) {
        unsigned int rev = dprev * 10 + f;            /* 10*last + next first */
        unsigned int diff = rev_digits(rev);          /* reversed digits */
        seq_t candidate = prev + (seq_t)diff;
        if (first_digit(candidate) == (seq_t)f) {
            if (candidate > best) best = candidate;
        }
    }
    return best;
}

/* public function: compute a(n) with memoisation */
/* Note: n is 1‑based; a(0) is undefined but returned 0 for safety */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_cache();
    if (n <= cache_size) return cache[n - 1];
    /* extend cache up to n (or until sequence stops) */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t next = compute_next(cache[cache_size - 1]);
        if (!next) {                 /* no further term – sequence ends */
            cache_size = i - 1;      /* last valid index */
            break;
        }
        ensure_capacity(i);
        cache[cache_size++] = next;
    }
    if (n <= cache_size) return cache[n - 1];
    return (seq_t)0;                 /* requested index beyond sequence end */
}
