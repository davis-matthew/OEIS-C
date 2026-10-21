/*
 * A254011  Expansion of (1 - x^18) / ((1 - x^5) * (1 - x^6) * (1 - x^9)) in
 *          powers of x.
 * https://oeis.org/A254011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-3) + a(n-5) - a(n-8) for n ≥ 8, * with initial
 *            values * a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, * a(5)=1,
 *            a(6)=1, a(7)=0. * This recurrence follows from the generating
 *            function * (1‑x¹⁸)/((1‑x⁵)(1‑x⁶)(1‑x⁹))
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cached terms */
static size_t cap = 0;           /* allocated cache size */
static idx_t computed_max = 0;   /* largest index already computed */
static int init_done = 0;        /* flag whether initial values written */

/* Ensure the cache can store at least indices 0 … n. */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)cap > n) return;
    size_t new_cap = cap ? cap : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) return;                 /* allocation failure → no change */
    cache = new_arr;
    cap   = new_cap;
}

/* Fill the cache with the initial eight terms and build subsequent terms
   up to index n.  The function is idempotent: it will not recompute terms
   that are already stored. */
static void build_up_to(idx_t n)
{
    if (!init_done) {
        /* write the eight known values */
        if (cap < 8) ensure_capacity(7);
        const seq_t init[8] = {1,0,0,0,0,1,1,0};
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
        computed_max = 7;
        init_done = 1;
    }

    if (computed_max >= n) return;        /* already computed up to n */

    /* compute terms from computed_max+1 up to n */
    size_t start = (size_t)computed_max + 1;
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t val = cache[i-5] + cache[i-3];
        val -= cache[i-8];
        cache[i] = val;
    }
    computed_max = n;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)(-1)) return 0;      /* guard against wrap‑around misuse */

    ensure_capacity(n);
    build_up_to(n);
    return cache[(size_t)n];
}
