/*
 * A383260  Expansion of e.g.f. f(x) * exp(f(x)), where f(x) = (exp(3*x) -
 *          1)/3.
 * https://oeis.org/A383260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * 3^(n-k) * Stirling2(n,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 0,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

static seq_t *a_cache = NULL;   /* a_wide(0..last_n) */
static size_t a_cap   = 0;

static seq_t *s_cache = NULL;   /* current Stirling row */
static size_t s_cap   = 0;

static idx_t last_n = (idx_t)-1;  /* last n computed */

/* ensure a_cache holds at least indices 0..n */
static void ensure_a_cap(idx_t n)
{
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *ptr = mem_resize(seq_t, a_cache, new_cap);
        if (ptr) { a_cache = ptr; a_cap = new_cap; }
    }
}

/* ensure s_cache can hold at least size n+1 */
static void ensure_s_cap(idx_t n)
{
    if ((size_t)n + 1 > s_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *ptr = mem_resize(seq_t, s_cache, new_cap);
        if (ptr) { s_cache = ptr; s_cap = new_cap; }
    }
}

seq_t a_wide(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (last_n == (idx_t)-1) {          /* first call */
        a_cache = mem_new(seq_t, 2);
        a_cap   = 2;
        a_cache[0] = (seq_t)0;

        s_cache = mem_new(seq_t, 1);
        s_cap   = 1;
        s_cache[0] = (seq_t)1;          /* S(0,0)=1 */

        last_n = 0;
    }

    if (n <= last_n) return a_cache[(size_t)n];

    for (idx_t m = last_n + 1; m <= n; ++m) {
        /* build Stirling row S(m,·) */
        ensure_s_cap(m);                /* allocate space for next row */

        seq_t *prev = s_cache;           /* old row */
        seq_t *cur  = mem_new(seq_t, (size_t)(m + 1));
        if (!cur) return a_cache[(size_t)m];  /* allocation failure unlikely */

        cur[0] = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t t = (seq_t)0;
            if (k < (idx_t)s_cap)   t += (seq_t)k * prev[k];
            if (k - 1 < (idx_t)s_cap) t += prev[k - 1];
            cur[k] = t;
        }

        /* compute a_wide(m) = Σ_{k=1..m} k * S(m,k) * 3^(m-k) */
        seq_t sum  = (seq_t)0;
        seq_t pow3 = (seq_t)1;               /* 3^0 */

        for (idx_t k = m; k >= 1; --k) {
            sum += (seq_t)k * cur[k] * pow3;
            pow3 *= (seq_t)3;
        }

        ensure_a_cap(m);
        a_cache[(size_t)m] = sum;

        /* replace previous row with current */
        mem_free(prev);
        s_cache = cur;
        s_cap   = (size_t)m + 1;
        last_n  = m;
    }

    return a_cache[(size_t)n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
