/*
 * A331265  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 31^2)^2 = y^2.
 * https://oeis.org/A331265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-5) - a(n-10) + 1922 (n ≥ 11) Initial values:
 *            a(1)=0, a(2)=279, a(3)=656, a(4)=1139, a(5)=1860, a(6)=2883,
 *            a(7)=4340, a(8)=6419, a(9)=9156, a(10)=13299 Recurrence is used
 *            for all larger n. The function works for any seq_t (built‑in
 *            integer or arbitrary‑precision class) and any idx_t =...
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 45, u64 n <= 108, u128 n <= 234,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;          /* 1‑based storage for terms   */
static size_t cache_cap = 0;         /* allocated size (including 0) */
static idx_t max_computed = 0;       /* largest n already filled */

/* Ensure the cache can hold indices up to n (inclusive).         */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;   /* already enough */

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    /* C++: allocate with new, default‑construct objects          */
    new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        delete [] cache;
    }
#else
    /* C: allocate with malloc (returns void*). Allowed only for
       built‑in integer seq_t.  The build that uses arbitrary‑precision
       will compile the above C++ branch.                      */
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free(cache);
    }
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the known first ten terms if not already done.       */
static void init_known_terms(void)
{
    if (max_computed >= 10) return;
    ensure_capacity(10);
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)279;
    cache[3] = (seq_t)656;
    cache[4] = (seq_t)1139;
    cache[5] = (seq_t)1860;
    cache[6] = (seq_t)2883;
    cache[7] = (seq_t)4340;
    cache[8] = (seq_t)6419;
    cache[9] = (seq_t)9156;
    cache[10] = (seq_t)13299;
    max_computed = 10;
}

/* Compute terms up to (and including) n using the recurrence.     */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 10) continue;          /* base cases already set */
        seq_t term = 6 * cache[(size_t)(i - 5)];
        term += (seq_t)1922;
        term -= cache[(size_t)(i - 10)];
        cache[(size_t)i] = term;
    }
    max_computed = n;
}

/* Public function required by the framework.                     */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1 */
    init_known_terms();
    ensure_capacity(n);
    compute_up_to(n);
    return cache[(size_t)n];
}
