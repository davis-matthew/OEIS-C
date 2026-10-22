/*
 * A374347  E.g.f. A(x) satisfies A(x) = A(x^3)^(1/3) * exp(3*x) with A(0)=1.
 * https://oeis.org/A374347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A374347
    a(n) = 3 * Σ_{k=1,3,9,… ≤ n} a(n‑k) * (n-1)! / (n-k)!   (a(0)=1)
    Computation is performed by dynamic programming using 64‑bit or
    arbitrary‑precision arithmetic supplied by the build system.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- utility to resize arrays ---------- */
static void ensure_capacity(void **p, size_t *cap, size_t elem_size, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? *cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    void *tmp = realloc(*p, new_cap * elem_size);
    if (!tmp) abort();
    *p = tmp;
    *cap = new_cap;
}

/* ---------- caches for sequence and factorials ---------- */
static seq_t *a_cache   = NULL;   /* a[0..]                     */
static size_t  a_cap    = 0;
static size_t  a_size   = 0;       /* number of pre‑computed a's */

static seq_t *fact_cache = NULL;  /* fact[0]=0! ... fact[n]      */
static size_t  fact_cap  = 0;
static size_t  fact_size = 0;     /* number of pre‑computed factorials */

/* ---------- ensure factorials up to max_f (inclusive) ---------- */
static void ensure_fact_cache(idx_t max_f)
{
    /* need factorials up to max_f, i.e. fact[0..max_f] */
    if ((size_t)max_f + 1 <= fact_size) return;
    ensure_capacity((void **)&fact_cache, &fact_cap, sizeof(seq_t), max_f + 1);

    if (fact_size == 0) {
        fact_cache[0] = (seq_t)1;
        fact_size = 1;
    }
    for (size_t i = fact_size; i <= (size_t)max_f; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_size = max_f + 1;
}

/* ---------- ensure sequence values up to n ---------- */
static void ensure_a_cache(idx_t n)
{
    if ((size_t)n + 1 <= a_size) return;          /* already computed */

    ensure_capacity((void **)&a_cache, &a_cap, sizeof(seq_t), n + 1);

    if (a_size == 0) {                            /* initialise a(0) */
        a_cache[0] = (seq_t)1;
        a_size = 1;
    }

    for (idx_t i = a_size; i <= n; ++i) {
        /* ensure factorials up to i-1 are available */
        ensure_fact_cache(i-1);

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; k *= 3) {       /* powers of 3 */
            idx_t j = i - k;                      /* previous index   */
            seq_t ratio = fact_cache[i-1] / fact_cache[j];   /* (i-1)!/(j)!  */
            seq_t term  = a_cache[j] * ratio;
            term *= (seq_t)3;                     /* multiply by 3    */
            sum += term;
        }
        a_cache[i] = sum;
    }
    a_size = (size_t)n + 1;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_a_cache(n);
    return a_cache[n];
}
