/*
 * A333443  Numbers k such that both k and k+1 are sums of two positive
 *          squares in 2 or more ways.
 * https://oeis.org/A333443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 490, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A333443 – numbers k such that both k and k+1
   are sums of two positive squares in at least two distinct ways.
   The implementation sieves to the smallest limit that guarantees
   at least n terms.  The counts for each number are kept in a byte
   array; a value is stored in the answer array only if both
   the number and the next one have a count of two or more.  The
   algorithm runs in about O(limit) time for a limit that grows
   roughly linearly with the number of requested terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* integer square root, floor of sqrt(n), without floating point */
static inline idx_t int_isqrt(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        /* mid*mid may overflow 64 bits, use 128‑bit */
        unsigned __int128 prod = (unsigned __int128)mid * (unsigned __int128)mid;
        if (prod <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* static sieve and result buffers */
static uint8_t *g_counts      = NULL;   /* number of unordered reps (positive) */
static size_t  g_counts_cap   = 0;
static seq_t  *g_terms        = NULL;   /* list of qualifying k values */
static size_t  g_terms_cap    = 0;
static size_t  g_terms_len    = 0;

/* recompute counts and terms up to a new limit */
static void recompute_counts(idx_t limit)
{
    if (limit == 0) return;

    /* resize counts array */
    if (g_counts_cap < (size_t)(limit + 1)) {
        uint8_t *tmp = (uint8_t *)realloc(g_counts, (size_t)(limit + 1));
        if (!tmp) abort();            /* OOM – unlikely in contest limits */
        g_counts = tmp;
        g_counts_cap = (size_t)(limit + 1);
    }
    memset(g_counts, 0, (size_t)(limit + 1));

    /* count unordered positive representations a <= b */
    idx_t sqrt_limit = int_isqrt(limit);
    for (idx_t a = 1; a <= sqrt_limit; ++a) {
        idx_t a2 = a * a;
        if (a2 > limit) break;
        idx_t rest = limit - a2;
        idx_t bmax = int_isqrt(rest);
        for (idx_t b = a; b <= bmax; ++b) {
            idx_t sum = a2 + b * b;
            if (sum <= limit) {
                if (g_counts[sum] < 255) g_counts[sum]++; /* avoid overflow */
            }
        }
    }

    /* resize terms array */
    if (g_terms_cap < (size_t)limit) {
        seq_t *tmp = (seq_t *)realloc(g_terms, (size_t)limit * sizeof(seq_t));
        if (!tmp) abort();            /* OOM – unlikely */
        g_terms = tmp;
        g_terms_cap = (size_t)limit;
    }
    g_terms_len = 0;

    /* collect qualifying k */
    for (idx_t k = 1; k < limit; ++k) {
        if (g_counts[k] >= 2 && g_counts[k + 1] >= 2) {
            g_terms[g_terms_len++] = (seq_t)k;
        }
    }
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)              /* safety for wrong index */
        return 0;

    static idx_t current_limit = 0;

    if (current_limit == 0)
        current_limit = 1024;   /* start with a modest limit */

    /* extend limit until enough terms have been found */
    while (g_terms_len < n) {
        recompute_counts(current_limit);
        if (g_terms_len >= n)
            break;
        /* prevent overflow on shift */
        if (current_limit > SIZE_MAX / 2) break;
        current_limit <<= 1;
    }

    if (g_terms_len < n) /* should not happen, but guard */
        return 0;
    return g_terms[n - 1];
}
