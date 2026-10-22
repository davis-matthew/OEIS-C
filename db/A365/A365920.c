/*
 * A365920  Greatest non-subset-sum of the prime indices of n, or 0 if there
 *          is none.
 * https://oeis.org/A365920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A365920: for a given n, factor n, form its multiset of
   prime indices (with multiplicity), find all subset sums of that multiset,
   and return the largest integer ≤ the total sum that is not one of those
   subset sums.  If every integer 0…sum is achievable, return 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------------------------------------------------------------------- */
/*  Prime list handling (sieve up to the greatest n seen so far).         */
/* ---------------------------------------------------------------------- */
static uint64_t   *prime_list = NULL;   /* 1‑based indices in prime_list[i] = prime(i+1)   */
static size_t     prime_cnt  = 0;       /* number of primes stored                     */
static idx_t      prime_limit = 0;      /* largest n for which primes have been built   */

/* Build a prime sieve up to new_limit (inclusive), replacing any old list. */
static void
ensure_primes(idx_t new_limit)
{
    if (new_limit <= prime_limit) return;          /* already enough */

    /* Rebuild */
    idx_t lim = new_limit;
    size_t sz = (size_t)lim + 1;                  /* 0..lim */

    /* Simple sieve: true = composite, false = prime candidate */
    bool *sieve = (bool *)calloc(sz, sizeof(bool));
    if (!sieve) return;                          /* allocation failure */

    for (idx_t i = 2; i * i <= lim; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= lim; j += i) sieve[j] = true;
        }
    }

    /* Count primes and allocate the prime list */
    size_t pcnt = 0;
    for (idx_t i = 2; i <= lim; ++i) if (!sieve[i]) ++pcnt;

    uint64_t *list = (uint64_t *)malloc(pcnt * sizeof(uint64_t));
    if (!list) { free(sieve); return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= lim; ++i) if (!sieve[i]) list[pos++] = (uint64_t)i;

    /* Replace old data */
    free(prime_list);
    prime_list  = list;
    prime_cnt   = pcnt;
    prime_limit = lim;

    free(sieve);
}

/* Binary search for the 1‑based index of a prime p in prime_list. */
static seq_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < prime_cnt && prime_list[lo] == p)
        return (seq_t)(lo + 1);   /* indices are 1‑based */
    return 0; /* should never happen for a prime factor of n */
}

/* ---------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* ---------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n=0, return 0 */

    /* Ensure we have all primes up to n for factorisation and indexing. */
    ensure_primes(n);

    /* Factorise n (trial division by primes).  Store prime indices. */
    idx_t tmp = n;
    seq_t indices[64];                  /* fits for n up to 10^10 comfortably */
    size_t idx_len = 0;
    seq_t sum = 0;                       /* A056239(n) */

    for (size_t pi = 0; pi < prime_cnt && (idx_t)prime_list[pi] * (idx_t)prime_list[pi] <= tmp; ++pi) {
        uint64_t p = prime_list[pi];
        if (tmp % p == 0) {
            seq_t idx = (seq_t)(pi + 1);          /* 1‑based index of p */
            do {
                if (idx_len < sizeof(indices) / sizeof(indices[0])) {
                    indices[idx_len++] = idx;
                } else {
                    /* Should not happen for n <= 10 000 (max 6 factors). */
                }
                sum += idx;
                tmp /= p;
            } while (tmp % p == 0);
        }
    }

    if (tmp > 1) {                      /* remaining prime factor > sqrt(n) */
        seq_t idx = prime_index(tmp);
        if (idx_len < sizeof(indices) / sizeof(indices[0])) {
            indices[idx_len++] = idx;
        }
        sum += idx;
    }

    /* If n == 1, no indices; the only reachable sum is 0. */
    if (idx_len == 0) return 0;

    /* Bitset for reachable subset sums up to 'sum' */
    size_t reach_len = (size_t)sum + 1;
    bool *reach = (bool *)calloc(reach_len, sizeof(bool));
    if (!reach) return 0;               /* allocation failure → 0 sentinel */

    reach[0] = true;

    for (size_t k = 0; k < idx_len; ++k) {
        size_t w = (size_t)indices[k];
        for (size_t s = reach_len - 1; s >= w; --s) {
            if (reach[s - w]) reach[s] = true;
        }
    }

    /* Find the greatest non‑reachable sum.  Scan downwards. */
    seq_t result = 0;
    for (size_t s = reach_len - 1; s > 0; --s) {
        if (!reach[s]) {
            result = (seq_t)s;
            break;
        }
    }

    free(reach);
    return result;
}
