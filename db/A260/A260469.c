/*
 * A260469  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000000 00000001 or 00010101.
 * https://oeis.org/A260469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed from the empirical recurrence a(n) =
 *            a(n-1)+a(n-2)+5*a(n-3)+a(n-4)+a(n-7)+2*a(n-8) -
 *            (3*a(n-5)+3*a(n-6)+a(n-9)+5*a(n-10)+5*a(n-11)
 *            +2*a(n-15)+2*a(n-16)) The recurrence holds for n > 17. Initial
 *            values for indices 1..20 are recorded below. No closed‐form
 *            formula is known; the func...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 49, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* array holding computed values       */
static idx_t cache_cap = 0;          /* allocated size of the array         */
static idx_t cache_last = 0;         /* highest index that has been computed */

/* Allocate or enlarge the cache so that indices up to n can be stored. */
static void ensure_capacity(idx_t n)
{
    /* Ensure the array has room for indices 0 .. n (inclusive). */
    if (n + 1 <= cache_cap) return;

    idx_t new_cap = n + 1;
    /* realloc returns void* in C++ – cast explicitly. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;

    /* Initialise the newly added elements to 0. */
    for (idx_t i = cache_last + 1; i < cache_cap; ++i) cache[i] = 0;
}

/* Initialise the cache with the first 20 terms. */
static void initial_terms(void)
{
    if (cache != NULL) return;           /* already initialised */

    cache_cap  = 21;                     /* indices 0..20 (0 unused) */
    cache      = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache) abort();                 /* allocation failure */
    cache_last = 20;

    static const seq_t init_vals[20] = {
        49UL,        /* a(1) */
        92UL,        /* a(2) */
        213UL,       /* a(3) */
        471UL,       /* a(4) */
        1085UL,      /* a(5) */
        2562UL,      /* a(6) */
        5891UL,      /* a(7) */
        13530UL,     /* a(8) */
        31372UL,     /* a(9) */
        72451UL,     /* a(10) */
        166919UL,    /* a(11) */
        385563UL,    /* a(12) */
        890572UL,    /* a(13) */
        2054927UL,   /* a(14) */
        4743628UL,   /* a(15) */
        10953016UL,  /* a(16) */
        25283529UL,  /* a(17) */
        58363941UL,  /* a(18) */
        134740753UL, /* a(19) */
        311052242UL  /* a(20) */
    };

    for (idx_t i = 1; i <= 20; ++i) cache[i] = init_vals[i - 1];
}

/* Compute all terms up to n that are not already in cache.  */
static void compute_to(idx_t n)
{
    if (cache_last >= n) return;          /* nothing to compute */

    ensure_capacity(n);

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* Indices <= 20 have already been set; recurrence starts at 21. */
        if (i <= 20) continue;

        seq_t pos = cache[i - 1];
        pos += cache[i - 2];
        pos += cache[i - 3] * (seq_t)5;
        pos += cache[i - 4];
        pos += cache[i - 7];
        pos += cache[i - 8] * (seq_t)2;

        seq_t neg = cache[i - 5] * (seq_t)3;
        neg += cache[i - 6] * (seq_t)3;
        neg += cache[i - 9];
        neg += cache[i - 10] * (seq_t)5;
        neg += cache[i - 11] * (seq_t)5;
        neg += cache[i - 15] * (seq_t)2;
        neg += cache[i - 16] * (seq_t)2;

        cache[i] = pos - neg;
    }
    cache_last = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    initial_terms();              /* make sure the cache is seeded   */
    compute_to(n);                /* compute missing terms if any    */

    return cache[n];              /* return the requested value      */
}
