/*
 * A352825  Number of nonfixed points y(i) != i, where y is the integer
 *          partition with Heinz number n.
 * https://oeis.org/A352825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(n) - A352824(n) (bigOmega(n) minus fixed points)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static cache of primes up to the largest value requested so far */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;
static idx_t    prime_limit  = 1;   /* maximum prime value stored */

/* extend the prime cache to cover all primes <= limit */
static void ensure_primes_up_to(idx_t limit)
{
    if (prime_limit >= limit) return;

    size_t sz = (size_t)limit + 1;
    char *is_comp = (char*)calloc(sz, 1);   /* 0 = prime, 1 = composite */
    if (!is_comp) return; /* silently fail; should not happen for small limits */

    for (size_t i = 2; i * i < sz; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j < sz; j += i)
                is_comp[j] = 1;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i < sz; ++i)
        if (!is_comp[i]) ++count;

    uint64_t *new_list = (uint64_t*)realloc(prime_list, count * sizeof(uint64_t));
    if (!new_list) {
        free(is_comp);
        return;
    }
    prime_list = new_list;
    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i)
        if (!is_comp[i]) prime_list[idx++] = i;

    prime_count = count;
    prime_limit = limit;
    free(is_comp);
}

/* binary search for the 1‑based index of a prime (always present in prime_list) */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid + 1;          /* 1‑based */
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;   /* should never happen */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1) = 0 */

    uint64_t nn = (uint64_t)n;
    ensure_primes_up_to(nn);

    /* arrays to hold distinct prime indices and their exponents */
    size_t max_cnt = prime_count;
    uint64_t *idxs = (uint64_t*)malloc(max_cnt * sizeof(uint64_t));
    uint64_t *exps = (uint64_t*)malloc(max_cnt * sizeof(uint64_t));
    if (!idxs || !exps) return 0; /* out of memory – unlikely */

    size_t cnt = 0;
    uint64_t rem = nn;
    /* trial division using the cached primes */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= rem; ++i) {
        uint64_t p = prime_list[i];
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            idxs[cnt]   = i + 1;      /* 1‑based prime index */
            exps[cnt++] = e;
        }
    }
    if (rem > 1) {  /* remaining factor is a prime */
        idxs[cnt]   = prime_index(rem);
        exps[cnt++] = 1;
    }

    /* sort by prime index in descending order (simple insertion sort) */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t key_idx  = idxs[i];
        uint64_t key_exp  = exps[i];
        size_t   jpos     = i;
        while (jpos > 0 && idxs[jpos - 1] < key_idx) {
            idxs[jpos] = idxs[jpos - 1];
            exps[jpos] = exps[jpos - 1];
            --jpos;
        }
        idxs[jpos] = key_idx;
        exps[jpos] = key_exp;
    }

    /* count total number of prime factors (Omega) and fixed points */
    seq_t total  = 0;
    seq_t fixed  = 0;
    seq_t pos = 1;    /* 1‑based position in the partition */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t j = (seq_t)idxs[i];
        uint64_t e = exps[i];
        for (uint64_t k = 0; k < e; ++k) {
            if (pos == j) ++fixed;
            ++pos;
        }
        total += (seq_t)e;
    }

    free(idxs);
    free(exps);

    return total - fixed;
}
