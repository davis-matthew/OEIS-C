/*
 * A381853  Expansion of 1/( Product_{k=0..3} (1 + (-1)^k * (2*k+1) * x) ).
 * https://oeis.org/A381853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A381853
   a(n) = 4*a(n-1) + 34*a(n-2) - 76*a(n-3) - 105*a(n-4)
   a(0)=1, a(1)=4, a(2)=50, a(3)=260
   The function uses lazy caching, so calls may be in any order. */

#include <stdlib.h>

/* Use an intermediate signed type wide enough for 64‑bit seq_t.
   Prefer __int128 if the compiler supports it; fall back to long long. */
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
typedef __int128 int128_t;
#else
typedef long long int128_t;
#endif

/* Initial values. */
static const seq_t init_vals[4] = { (seq_t)1, (seq_t)4, (seq_t)50,
                                   (seq_t)260 };

/* Cache of computed terms.  static variables keep state between calls. */
static seq_t  *cache    = NULL;        /* array holding terms 0..max_n */
static idx_t   max_n    = (idx_t)-1;    /* highest computed index */

/* Ensure cache contains terms up to n.  Reallocates as needed. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate for the first four terms */
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (cache == NULL) return; /* allocation failed – caller should handle */
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
        max_n   = 3;
    }

    if (n <= max_n) return;                 /* already computed */

    /* enlarge cache to hold up to n */
    size_t newsize = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
    if (tmp == NULL) return; /* allocation failed – caller should handle */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        int128_t term =
            (int128_t)cache[i-1] * (int128_t)4   /* 4*a(n-1)  */
          + (int128_t)cache[i-2] * (int128_t)34  /* +34*a(n-2) */
          - (int128_t)cache[i-3] * (int128_t)76  /* -76*a(n-3) */
          - (int128_t)cache[i-4] * (int128_t)105;/* -105*a(n-4)*/
        cache[i] = (seq_t)term;                /* cast back – fits in seq_t */
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    /* Small n: return the known initial values. */
    if (n < 4) return init_vals[n];

    ensure_cache(n);
    return cache[n];
}
