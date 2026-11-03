/*
 * A374353  Least k such that prime(n) mod k = n or 0 if no such k exists.
 * https://oeis.org/A374353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that prime(n) mod k = n (0 if no such k
 *            exists). Note that prime(n) is the n‑th prime, with 1‑based
 *            indexing
 * Range:     u8 n <= 66, u16 n <= 7193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9461 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Return the smallest divisor of diff that is greater than n,
   or 0 if none exists.  diff = prime(n) – n. */
static seq_t find_min_div_greater(seq_t diff, seq_t n)
{
    seq_t best = 0;
    /* Simple loop up to sqrt(diff). Since diff is small (≈n log n),
       this loop is trivial. */
    for (idx_t i = 1; (seq_t)i * (seq_t)i <= diff; ++i) {
        seq_t i_seq = (seq_t)i;
        if (diff % i_seq == 0) {
            seq_t d1 = i_seq;
            seq_t d2 = diff / i_seq;
            if (d1 > n && (best == 0 || d1 < best))
                best = d1;
            if (d2 > n && (best == 0 || d2 < best))
                best = d2;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1.  For n <= 4 the answer is 0. */
    if (n <= 4) return 0;

    /* Get the n-th prime.  get_prime(1)=2, etc.  The function returns
       a uint32_t, which is safe up to very large n for our use case. */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Compute difference prime(n) – n. */
    seq_t diff = p - (seq_t)n;

    /* Find least divisor > n.  If none, return 0. */
    return find_min_div_greater(diff, (seq_t)n);
}
