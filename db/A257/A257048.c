/*
 * A257048  Numbers n for which the sum of their prime factors (with
 *          repetition) divides the Euler totient function.
 * https://oeis.org/A257048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for integers n such that the sum of their prime factors
 * (with multiplicity) divides φ(n).  Returns the n-th such integer
 * (offset 1).  The function uses a cache so that calls in any order
 * are fast, and works for any seq_t/idx_t types supplied by the
 * build. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of the sequence function. */
seq_t a (idx_t n);

/* Helper: determines whether a given unsigned 64‑bit integer m satisfies
 * the condition: sum of prime factors of m (with repetition) divides φ(m). */
static bool good (uint64_t m)
{
    if (m <= 1U)
        return false;

    uint64_t rem = m;
    seq_t sum   = (seq_t)0;
    seq_t phi   = (seq_t)m;

    /* Handle factor 2 */
    if ((rem & 1U) == 0U) {
        unsigned int cnt = 0;
        while ((rem & 1U) == 0U) {
            rem >>= 1U;
            sum += (seq_t)2;
            ++cnt;
        }
        if (cnt > 0)
            phi = phi / (seq_t)2 * (seq_t)1; /* (2-1)=1 */
    }

    /* Handle odd factors */
    for (uint64_t p = 3U; p <= rem / p; p += 2U) {
        if (rem % p != 0U)
            continue;
        unsigned int cnt = 0;
        while (rem % p == 0U) {
            rem /= p;
            sum += (seq_t)p;
            ++cnt;
        }
        if (cnt > 0)
            phi = phi / (seq_t)p * (seq_t)(p - 1);
    }

    /* Remaining prime factor */
    if (rem > 1U) {
        sum += (seq_t)rem;
        phi = phi / (seq_t)rem * (seq_t)(rem - 1);
    }

    return (phi % sum == (seq_t)0);
}

/* The sequence function.  It caches all values found so far. */
seq_t a (idx_t n)
{
    /* Cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_checked = 1U;   /* last candidate examined */

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        ++last_checked;
        if (good(last_checked)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0U) ? 128U : cache_cap * 2U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure: return 0 as safe fallback */
                    return (seq_t)0;
                }
                cache         = tmp;
                cache_cap     = new_cap;
            }
            cache[cache_len] = (seq_t)last_checked;
            ++cache_len;
        }
    }
    return cache[n - 1U];
}
