/*
 * A325390  Heinz number of the negated differences plus one of the integer
 *          partition with Heinz number n (with the last part taken to be 0).
 * https://oeis.org/A325390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Heinz number of the negated differences plus one of
 *            the integer partition with Heinz number n (with the last part
 *            taken to be 0). The partition is obtained from the prime
 *            factorization of n, mapping each prime factor to its index in
 *            the sequence of primes
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

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

static size_t binary_search_idx(const uint64_t *arr, size_t n, uint64_t val) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return lo; /* first index >= val */
}

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static uint64_t prime_limit = 0;

static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= prime_limit) return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

static int cmp_desc(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return 1;
    if (va > vb) return -1;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;                /* undefined but kept safe */
    if (n == 1) return 1;                /* zero‑factor partition */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);
    if (pcnt == 0) return 1;             /* n==1 case already handled */

    uint64_t idxs[64];
    size_t ilen = 0;

    /* collect indices corresponding to prime factors */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        ensure_primes_up_to(p);
        size_t pos = binary_search_idx(prime_list, prime_cnt, p);
        if (pos == prime_cnt || prime_list[pos] != p) continue; /* safety */
        uint64_t idx = pos + 1; /* 1‑based index */
        for (uint64_t e = 0; e < exps[i]; ++e) {
            idxs[ilen++] = idx;
        }
    }

    /* sort indices descending */
    if (ilen > 1) qsort(idxs, ilen, sizeof(uint64_t), cmp_desc);

    /* compute Heinz number of negated differences + 1 */
    seq_t result = 1;
    for (size_t i = 0; i < ilen; ++i) {
        uint64_t cur = idxs[i];
        uint64_t nxt = (i + 1 < ilen) ? idxs[i + 1] : 0;
        uint64_t t   = cur - nxt + 1;          /* negated diff + 1 */
        seq_t prime_val = (seq_t)get_prime((idx_t)t);
        result *= prime_val;
    }
    return result;
}
