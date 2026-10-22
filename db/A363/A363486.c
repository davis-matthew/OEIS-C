/*
 * A363486  Low mode in the multiset of prime indices of n.
 * https://oeis.org/A363486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = low mode of the multiset of prime indices of n (OEIS
 *            A363486). For n=1 the multiset is empty and a(1)=0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t, uint64_t **, size_t *) */

static uint64_t *prime_list = NULL;  /* 0‑based array of primes, 1‑based indices */
static size_t prime_list_cnt = 0;

/* Extend the prime list so that it contains every prime <= limit */
static void
ensure_prime_limit(idx_t limit)
{
    if (prime_list != NULL && prime_list_cnt > 0 &&
        prime_list[prime_list_cnt - 1] >= limit)
        return;                           /* already sufficient */

    uint64_t *newlist = NULL;
    size_t   newcnt  = 0;
    build_primes_upto(limit, &newlist, &newcnt);

    free(prime_list);
    prime_list     = newlist;
    prime_list_cnt = newcnt;
}

/* Return the 1‑based index of the prime p in the prime list.
   The list is grown if necessary. */
static idx_t
prime_index(idx_t p)
{
    if (prime_list == NULL || prime_list_cnt == 0)
        ensure_prime_limit(p * 2);  /* guarantee we have primes at least up to p */

    if (prime_list[prime_list_cnt - 1] < p)
        ensure_prime_limit(p);      /* extend to include p itself */

    /* Binary search for p */
    size_t lo = 0, hi = prime_list_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] == p)
            return (idx_t)(mid + 1);           /* indices are 1‑based */
        else if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;   /* should never happen */
}

/* Helper structure: (index, multiplicity) */
struct fact_entry {
    idx_t idx;
    unsigned cnt;
};

seq_t a(idx_t n)
{
    if (n == 1)          /* empty multiset */
        return (seq_t)0;

    /* Ensure we have at least some primes to work with */
    ensure_prime_limit(2);

    /* Vector for distinct prime indices */
    struct fact_entry entries[64];
    size_t entries_len = 0;

    /* Factor n using trial division */
    idx_t temp = n;
    for (size_t pi = 0; pi < prime_list_cnt; ++pi) {
        uint64_t p = prime_list[pi];
        idx_t p_idx = (idx_t)(pi + 1);
        if ((idx_t)p * (idx_t)p > temp) break;

        while ((temp % p) == 0) {
            temp /= p;
            /* Update count for this prime index */
            bool found = false;
            for (size_t k = 0; k < entries_len; ++k) {
                if (entries[k].idx == p_idx) {
                    ++entries[k].cnt;
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (entries_len == 64)   /* should never happen for small n */
                    break;
                entries[entries_len].idx = p_idx;
                entries[entries_len].cnt = 1;
                ++entries_len;
            }
        }
    }

    /* Remaining factor > 1 (prime) */
    if (temp > 1) {
        /* Ensure prime list contains this remaining prime */
        ensure_prime_limit(temp);
        idx_t p_idx = prime_index(temp);

        bool found = false;
        for (size_t k = 0; k < entries_len; ++k) {
            if (entries[k].idx == p_idx) {
                ++entries[k].cnt;
                found = true;
                break;
            }
        }
        if (!found) {
            if (entries_len == 64)   /* should never happen for small n */
                return (seq_t)0;
            entries[entries_len].idx = p_idx;
            entries[entries_len].cnt = 1;
            ++entries_len;
        }
    }

    /* Find the mode (max count); tie -> smallest index */
    unsigned maxcnt = 0;
    idx_t bestidx = (idx_t)-1;
    for (size_t k = 0; k < entries_len; ++k) {
        if (entries[k].cnt > maxcnt ||
            (entries[k].cnt == maxcnt && entries[k].idx < bestidx)) {
            maxcnt = entries[k].cnt;
            bestidx = entries[k].idx;
        }
    }

    return (seq_t)bestidx;
}
