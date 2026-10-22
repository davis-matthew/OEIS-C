/*
 * A370861  Numbers m such that c(0) < c(1) >= c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th integer m such that in the base‑3 representation of m
 * the number of 0‑digits is less than the number of 1‑digits, and
 * the number of 1‑digits is greater than or equal to the number of
 * 2‑digits.  The computation is a simple search with memoisation
 * of all qualifying numbers found so far. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* memoised sequence values   */
static size_t cache_len = 0;  /* number of items stored     */
static size_t cache_cap = 0;  /* allocated capacity         */
static seq_t last_checked = 0;/* last m examined            */

/* Return true if m satisfies c(0) < c(1) >= c(2).  The ternary
 * representation of m is used; leading zeros are not counted. */
static bool qualifies(seq_t m)
{
    unsigned cnt[3] = {0, 0, 0};
    seq_t t = m;
    while (t > 0) {
        unsigned d = (unsigned)(t % 3);
        ++cnt[d];
        t /= 3;
    }
    return (cnt[0] < cnt[1]) && (cnt[1] >= cnt[2]);
}

/* Ensure the cache holds at least n qualifying values.
 * When more capacity is needed, grow in powers of two. */
static void ensure_size(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* search for the next qualifying m */
        seq_t m;
        for (m = last_checked + 1; ; ++m) {
            if (qualifies(m)) {
                /* expand capacity if necessary */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    while (new_cap < cache_len + 1)
                        new_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) /* allocation failed – terminate */
                        abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = m;
                last_checked = m;
                break;
            }
            last_checked = m;
        }
    }
}

/* Public sequence function.  n >= 1; n == 0 returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined but harmless  */
    ensure_size(n);
    return cache[n - 1];
}
