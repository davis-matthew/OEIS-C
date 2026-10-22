/*
 * A303685  Number of nX3 0..1 arrays with every element unequal to 2, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+6·a(n-2)-3·a(n-3)-9·a(n-4)+7·a(n-5)+3·a(n-6)
 *            -20·a(n-7)-3·a(n-8)+12·a(n-9)-12·a(n-11)-3·a(n-12)
 *            +2·a(n-13)-5·a(n-14)+a(n-15)+2·a(n-16) initial values: a(1)=0,
 *            a(2)=5, a(3)=2, a(4)=20, a(5)=27, a(6)=108, a(7)=191, a(8)=599,
 *            a(9)=1261, a(10)=3503, a(11)=8019, a(12)=20956, a(13)=49932,...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* dynamic cache for terms, index starts at 1 */
static idx_t max_n = 0;        /* highest n that has been computed */
static size_t cache_cap = 0;   /* current allocation size (>= max_n+1) */

/* Allocate or grow the cache to hold indices up to 'n' (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;          /* already large enough */
    size_t new_cap = cache_cap ? cache_cap << 1 : 19;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;

    /* realloc with an explicit cast so this compiles as C++ too */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);/* allocation failure */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 18 terms supplied in the OEIS entry */
static void init_cache(void)
{
    if (cache) return;                 /* already initialised */

    ensure_capacity(18);
    static const seq_t init[19] = {
        0, /* a(0) unused */
        0, 5, 2, 20, 27, 108, 191, 599, 1261,
        3503, 8019, 20956, 49932, 126833,
        308197, 772321, 1894368, 4716189
    };
    for (size_t i = 1; i <= 18; ++i) cache[i] = init[i];
    max_n = 18;
}

/* Compute the term a(n) assuming all earlier terms up to n-1 exist */
static seq_t compute_term(idx_t n)
{
    seq_t res = cache[n-1];
    res += (seq_t)6 * cache[n-2];
    res -= (seq_t)3 * cache[n-3];
    res -= (seq_t)9 * cache[n-4];
    res += (seq_t)7 * cache[n-5];
    res += (seq_t)3 * cache[n-6];
    res -= (seq_t)20 * cache[n-7];
    res -= (seq_t)3 * cache[n-8];
    res += (seq_t)12 * cache[n-9];
    res -= (seq_t)12 * cache[n-11];
    res -= (seq_t)3 * cache[n-12];
    res += (seq_t)2 * cache[n-13];
    res -= (seq_t)5 * cache[n-14];
    res += cache[n-15];
    res += (seq_t)2 * cache[n-16];
    return res;
}

/* Public entry point – computes a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n < 1 */

    init_cache();                  /* ensure cache is ready */

    if (n <= max_n) return cache[n];

    ensure_capacity(n);            /* grow buffer if necessary */

    /* Compute successive terms up to desired index */
    for (idx_t i = max_n + 1; i <= n; ++i)
        cache[i] = compute_term(i);

    max_n = n;
    return cache[n];
}
