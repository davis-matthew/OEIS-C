/*
 * A283891  Relative of Hofstadter Q-sequence.
 * https://oeis.org/A283891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9647, u32 n <= 9647, u64 n <= 9647,
 *            u128 n <= 9647, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283891 – Relative of Hofstadter Q‑sequence
   Definition:
        a(n) = max(0, n+10000)                 for n <= 0
        a(n) = a(n-a(n-1)) + a(n-a(n-2))
               + a(n-a(n-3)) + a(n-a(n-4))
               + a(n-a(n-5)) + a(n-a(n-6))
               + a(n-a(n-7))                for n > 0
   Offset: 1  (a(1) is the first term listed)
*/

#include <stdlib.h>
#include <stdint.h>

/* cache for computed values (1‑based).  index 0 is unused but kept for
   allocation simplicity. */
static seq_t *cache = NULL;         /* array of a[n] */
static size_t cache_cap = 0;        /* allocated capacity (>= computed_max+1) */
static idx_t computed_max = 0;      /* largest n for which cache[n] is valid */

/* return a(m) for any m (signed), using the base rule for m <= 0
   and the cache for m >= 1.  m is given as int64_t to allow negative
   values without underflow. */
static inline seq_t get_a_signed(int64_t m)
{
    if (m >= 1) {
        /* no bounds check needed – cache up to computed_max covers this case */
        return cache[(size_t)m];
    } else {
        int64_t val = m + 10000;      /* max(0, m+10000) */
        return (val > 0) ? (seq_t)val : (seq_t)0;
    }
}

/* ensure that a[1..n] are computed and cached */
static void ensure_cache(idx_t n)
{
    if (n <= computed_max) return;

    /* grow the cache array if required */
    size_t new_cap = (size_t)n + 1;   /* index 0 reserved */
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms in increasing order */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 7; ++k) {
            int64_t prev_idx = (int64_t)i - k;          /* i-k */
            seq_t a_prev = get_a_signed(prev_idx);      /* a(i-k) */
            int64_t j = (int64_t)i - (int64_t)a_prev;   /* i - a(i-k) */
            seq_t term = get_a_signed(j);               /* a(j) */
            sum += term;
        }
        cache[(size_t)i] = sum;
    }

    computed_max = n;
}

/* public sequence function – external linkage.  n is assumed >= 1. */
seq_t a(idx_t n)
{
    /* a(0) is defined by the base rule, we seldom call it but handle it. */
    if (n == 0) return (seq_t)10000;

    ensure_cache(n);
    return cache[(size_t)n];
}
