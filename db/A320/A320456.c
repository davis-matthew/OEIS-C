/*
 * A320456  Numbers whose multiset multisystem spans an initial interval of
 *          positive integers.
 * https://oeis.org/A320456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer whose multiset multisystem spans an initial
 *            interval of positive integers. The test is performed by
 *            factoring a candidate integer, collecting the indices of its
 *            prime factors (the "root indices"), then for each root index
 *            collecting the indices of the prime factors of that inde...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/factor_u64.h"

static uint64_t *g_primes = NULL; /* primes array */
static size_t     g_prime_cnt = 0;

static void init_primes(void) {
    if (g_primes) return;
    /* Adjust the limit if you need more primes for larger sequence terms. */
    const idx_t LIMIT = (idx_t)5000000; /* 5 million primes are enough for first 10 k terms */
    build_primes(LIMIT, &g_primes, &g_prime_cnt);
}

static inline idx_t prime_index_lookup(uint64_t p) {
    /* Binary search in g_primes; assumes p is prime and within the computed range */
    idx_t lo = 0, hi = g_prime_cnt;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (g_primes[mid] == (uint64_t)p) return mid + 1;
        if (g_primes[mid] < (uint64_t)p) lo = mid + 1;
        else hi = mid;
    }
    /* Should never happen with a sufficient prime limit. */
    return 0;
}

static int cmp_idx(const void *a, const void *b) {
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static bool qualifies(uint64_t n) {
    if (n == 1) return true; /* by definition */

    /* Step 1: factor n into distinct primes and collect their indices */
    uint64_t pf[32], exp[32];
    size_t pf_cnt;
    factor_u64(n, pf, exp, &pf_cnt);

    idx_t root_idx[32], root_cnt = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t ind = prime_index_lookup(pf[i]); /* index of prime pf[i] */
        /* store unique indices */
        bool seen = false;
        for (size_t j = 0; j < root_cnt; ++j)
            if (root_idx[j] == ind) { seen = true; break; }
        if (!seen) root_idx[root_cnt++] = ind;
    }

    /* Step 2: for each root index collect prime indices of that index */
    idx_t t_idx[64], t_cnt = 0;
    for (size_t i = 0; i < root_cnt; ++i) {
        uint64_t r = (uint64_t)root_idx[i];
        uint64_t rf[16], re[16];
        size_t rf_cnt;
        factor_u64(r, rf, re, &rf_cnt);

        for (size_t j = 0; j < rf_cnt; ++j) {
            idx_t ti = prime_index_lookup(rf[j]); /* index of prime rf[j] */
            bool seen = false;
            for (size_t k = 0; k < t_cnt; ++k)
                if (t_idx[k] == ti) { seen = true; break; }
            if (!seen) t_idx[t_cnt++] = ti;
        }
    }

    if (t_cnt == 0) return true; /* empty set -> initial interval of length 0 */

    /* Step 3: check that t_idx is exactly {1,2,…,max} */
    idx_t max_t = 0;
    for (size_t i = 0; i < t_cnt; ++i)
        if (t_idx[i] > max_t) max_t = t_idx[i];

    qsort(t_idx, t_cnt, sizeof(idx_t), cmp_idx);

    for (size_t i = 0; i < t_cnt; ++i) {
        if (t_idx[i] != (idx_t)(i + 1)) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    init_primes();

    idx_t count = 0;
    uint64_t candidate = 1;
    while (count < n) {
        if (qualifies(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
    /* unreachable */
    return (seq_t)0;
}
