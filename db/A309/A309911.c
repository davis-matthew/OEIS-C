/*
 * A309911  a(n) = Sum_{k=1..n} Stirling2(n,k) * floor(n/k).
 * https://oeis.org/A309911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} S(n,k) * ⌊n/k⌋ (OEIS A309911)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* very small static state for memoisation */
static seq_t *a_cache   = NULL;   /* a_cache[i] = a(i)          */
static idx_t  a_cap     = 0;      /* size of a_cache array      */
static seq_t *s         = NULL;   /* s[k] = S(current_n,k)     */
static size_t s_cap     = 0;      /* size of s array            */
static idx_t  max_n     = 0;      /* largest n already computed */

/* Ensure that a_cache has room for indices up to 'need' (inclusive) */
static void ensure_a_capacity(idx_t need)
{
    if (a_cap < (size_t)need + 1) {
        size_t new_cap = a_cap ? a_cap : 8;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

/* Ensure that s has room for indices up to 'need' (inclusive) */
static void ensure_s_capacity(idx_t need)
{
    if (s_cap < (size_t)need + 1) {
        size_t new_cap = s_cap ? s_cap : 8;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(s, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        s = tmp;
        s_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n=0 is outside of the OEIS offset */

    /* Return a memoised value if available */
    if (max_n >= n) return a_cache[n];

    /* Initialise state on first call (n >= 1) */
    if (max_n == 0) {
        ensure_s_capacity(1);
        s[0] = 0;          /* S(1,0) = 0 for n>0 */
        s[1] = 1;          /* S(1,1) = 1 */
        ensure_a_capacity(1);
        a_cache[1] = 1;
        max_n = 1;
    }

    /* Compute consecutive values up to the requested n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        ensure_s_capacity(cur);
        s[cur] = 0;                      /* will become S(cur,cur) */

        /* Update Stirling numbers of the second kind in reverse order */
        for (idx_t k = cur; k != 0; --k) {
            seq_t new_val = s[k] * (seq_t)k + s[k - 1];
            s[k] = new_val;
        }

        /* Compute a(cur) = Σ_{k=1}^{cur} S(cur,k) * ⌊cur/k⌋ */
        seq_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            sum += s[k] * (seq_t)(cur / k);
        }

        ensure_a_capacity(cur);
        a_cache[cur] = sum;
        max_n = cur;
    }

    return a_cache[n];
}
