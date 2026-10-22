/*
 * A357268  If n is a power of 2, a(n) = n. Otherwise, if 2^j is the greatest
 *          power of 2 not exceeding n, and if k = n - 2^j, then a(n) is the
 *          smallest m*a(k) which has not occurred already, where m is an odd
 *          number.
 * https://oeis.org/A357268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 500, u32 n <= 9847, u64 n <= 9847,
 *            u128 n <= 9847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A357268
 * a(n) = n, if n is a power of 2
 * otherwise
 *   let j = ⌊log₂(n)⌋,  pw = 2^j,  k = n - pw
 *   a(n) = the smallest m·a(k) (m odd) that has not appeared before
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* internal cache of all computed terms (0‑based index) */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

/* sorted list of all values that have appeared so far */
static seq_t *seen = NULL;
static size_t seen_cap = 0;
static size_t seen_len = 0;

/* make sure seq_cache can hold at least 'need' elements */
static void ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* make sure seen can hold at least 'need' elements */
static void ensure_seen_cap(size_t need)
{
    if (need <= seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seen = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
    seen_cap = new_cap;
}

/* binary search in the sorted 'seen' array */
static bool seen_contains(seq_t val)
{
    size_t lo = 0, hi = seen_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (seen[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < seen_len && seen[lo] == val);
}

/* insert val into the sorted 'seen' array (assumes val not present) */
static void seen_insert(seq_t val)
{
    size_t pos = 0;
    while (pos < seen_len && seen[pos] < val) pos++;
    ensure_seen_cap(seen_len + 1);
    for (size_t j = seen_len; j > pos; --j)
        seen[j] = seen[j-1];
    seen[pos] = val;
    ++seen_len;
}

/* compute floor(log2(v)) for a uint64_t value v >= 1 */
static inline idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v >> 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0 */

    ensure_seq_cap(n);

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t val;
        if ((i & (i - 1)) == 0) {         /* power of two */
            val = (seq_t)i;
        } else {
            idx_t pw = (idx_t)1ULL << floor_log2_uint64(i);
            idx_t k  = i - pw;            /* 1 ≤ k < i */
            seq_t base = seq_cache[k - 1];/* a(k) */

            /* find smallest odd m with m*base new */
            seq_t m = (seq_t)1;
            for (;;) {
                seq_t prod = m * base;
                if (!seen_contains(prod))
                    break;
                m += (seq_t)2;
            }
            val = m * base;
        }
        seq_cache[i - 1] = val;
        seen_insert(val);
    }

    seq_len = n;
    return seq_cache[n - 1];
}
