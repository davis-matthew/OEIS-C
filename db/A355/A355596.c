/*
 * A355596  Numbers all of whose divisors are alternating numbers (A030141).
 * https://oeis.org/A355596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_alternating_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n‑th integer whose every divisor is an alternating number. */
/* The function enumerates integers, keeps only those that are alternating
 * and for which all divisors are alternating, storing the found terms
 * in a static cache so that unordered calls to a(idx_t n) return the
 * correct value. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_alternating_seq_t.h"

/* Recursive enumeration of all divisors of a uint64_t number.
 * Returns false immediately upon finding a divisor that is not
 * an alternating number. */
static bool dfs_divs(size_t idx, size_t cnt,
                     const uint64_t *pr, const uint64_t *ex,
                     uint64_t cur)
{
    if (idx == cnt) {
        return is_alternating_seq_t((seq_t)cur);
    }
    uint64_t val = cur;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        if (!dfs_divs(idx + 1, cnt, pr, ex, val))
            return false;
        val *= pr[idx];
    }
    return true;
}

/* Test whether every divisor of n (uint64_t) is an alternating number. */
static bool all_divisors_alternating(uint64_t n)
{
    /* Special case: n == 1 has only divisor 1 (alternating). */
    if (n == 1) return true;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t m = n;

    /* Trial division to factor n. */
    uint64_t p = 2;
    while (p * p <= m) {
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[cnt] = p;
            ex[cnt] = e;
            ++cnt;
        }
        p += (p == 2 ? 1 : 2);
    }
    if (m > 1) {          /* Remaining prime factor. */
        pr[cnt] = m;
        ex[cnt] = 1;
        ++cnt;
    }

    return dfs_divs(0, cnt, pr, ex, 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1;
    static idx_t terms_found = 0;

    while (terms_found < n) {
        /* Skip non‑alternating numbers first. */
        while (!is_alternating_seq_t((seq_t)next_candidate))
            ++next_candidate;

        if (all_divisors_alternating(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
            ++terms_found;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
