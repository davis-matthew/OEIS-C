/*
 * A381991  Numbers whose prime indices have a unique multiset partition into
 *          constant multisets with distinct sums.
 * https://oeis.org/A381991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 1247, u32 n <= 1247, u64 n <= 1247,
 *            u128 n <= 1247, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A381991
   A search algorithm that tests each integer for the unique multiset
   partition property by exhaustive enumeration of integer partitions
   of the prime exponents.  The sequence values are generated
   incrementally up to the index requested.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Include the utility that reallocates a seq_t array. */
#include "../utils/grow_seq_array.h"

/* ------------------------------------------------------------- */
/* Global tables for primes and prime indices  */
/* ------------------------------------------------------------- */
#define PRIME_LIMIT 200000          /* should cover at least the first 10k terms */

static uint32_t *g_prime_list = NULL;
static size_t g_prime_count = 0;
static size_t g_prime_cap = 0;

static uint32_t *g_prime_index_map = NULL;   /* map: prime -> 1‑based index */

/* ------------------------------------------------------------- */
/* Helper: sieve primes up to PRIME_LIMIT and populate tables   */
/* ------------------------------------------------------------- */
static void init_primes(void)
{
    if (g_prime_list != NULL) return;   /* already done */

    /* allocate maps */
    g_prime_index_map = (uint32_t *)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    g_prime_cap = PRIME_LIMIT / 10 + 10;  /* rough estimate */
    g_prime_list = (uint32_t *)malloc(g_prime_cap * sizeof(uint32_t));

    /* simple sieve */
    uint8_t *is_comp = (uint8_t *)calloc(PRIME_LIMIT + 1, sizeof(uint8_t));
    for (uint32_t p = 2; (uint64_t)p * p <= PRIME_LIMIT; ++p) {
        if (!is_comp[p]) {
            for (uint64_t q = (uint64_t)p * p; q <= PRIME_LIMIT; q += p)
                is_comp[q] = 1;
        }
    }
    for (uint32_t p = 2; p <= PRIME_LIMIT; ++p) {
        if (!is_comp[p]) {
            if (g_prime_count == g_prime_cap) {
                g_prime_cap = g_prime_cap * 2;
                g_prime_list = (uint32_t *)realloc(g_prime_list,
                        g_prime_cap * sizeof(uint32_t));
            }
            g_prime_list[g_prime_count++] = p;
            g_prime_index_map[p] = (uint32_t)(g_prime_count);  /* 1‑based */
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------- */
/* Factorisation using the prime table.                          */
/* ------------------------------------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *fact, int *exps, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = n;
    for (size_t i = 0; i < g_prime_count && (uint64_t)g_prime_list[i] * g_prime_list[i] <= m; ++i) {
        uint64_t p = g_prime_list[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            fact[(*cnt)] = p;
            exps[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (m > 1) {
        fact[(*cnt)] = m;
        exps[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------- */
/* Uniqueness test by exhaustive partition enumeration            */
/* ------------------------------------------------------------- */

#define MAX_EXP 20                /* maximum exponent considered */
#define MAX_GROUPS 32             /* maximum number of groups in a partition */

/* Recursive helpers */
static bool dfs_index(int idx, const int *exps, const int *indices,
                      int m, uint64_t *used, int used_cnt,
                      int *found_cnt, bool *stop);

static void try_partition(int idx, int remaining, int min_part, int depth,
                          int *part, const int *exps, const int *indices,
                          int m, uint64_t *used, int used_cnt,
                          int *found_cnt, bool *stop)
{
    if (*stop) return;

    if (remaining == 0) {
        /* evaluate current partition 'part[0..depth-1]' for index idx */
        uint64_t local[MAX_GROUPS];
        int local_cnt = 0;
        bool ok = true;
        for (int i = 0; i < depth && ok; ++i) {
            uint64_t s = (uint64_t)indices[idx] * (uint64_t)part[i];
            /* check against global used sums */
            for (int j = 0; j < used_cnt; ++j) {
                if (used[j] == s) { ok = false; break; }
            }
            /* check against local sums (duplicate within same index) */
            for (int j = 0; j < local_cnt; ++j) {
                if (local[j] == s) { ok = false; break; }
            }
            if (ok) local[local_cnt++] = s;
        }
        if (!ok) return;  /* discard this partition */

        /* add local sums to used */
        for (int i = 0; i < local_cnt; ++i)
            used[used_cnt + i] = local[i];
        int new_used_cnt = used_cnt + local_cnt;

        /* recurse to next index */
        dfs_index(idx + 1, exps, indices, m, used, new_used_cnt,
                  found_cnt, stop);

        /* no need to remove local sums; they will be overwritten */
        return;
    }

    for (int k = min_part; k <= remaining; ++k) {
        part[depth] = k;
        if (*stop) return;
        try_partition(idx, remaining - k, k, depth + 1,
                      part, exps, indices, m, used, used_cnt,
                      found_cnt, stop);
    }
}

static bool dfs_index(int idx, const int *exps, const int *indices,
                      int m, uint64_t *used, int used_cnt,
                      int *found_cnt, bool *stop)
{
    if (*stop) return true;
    if (idx == m) {
        (*found_cnt)++;
        if (*found_cnt > 1) *stop = true;
        return *stop;
    }

    int e = exps[idx];
    int part[MAX_EXP];
    try_partition(idx, e, 1, 0, part, exps, indices, m,
                  used, used_cnt, found_cnt, stop);
    return *stop;
}

static bool has_unique_partition(uint64_t num)
{
    if (num == 1) return true;   /* 1 is always included */

    uint64_t pf[64];
    int    ep[64];
    size_t cnt;
    factor_uint64(num, pf, ep, &cnt);
    if (cnt == 0) return true;   /* shouldn't happen */

    int indices[64];
    int exps[64];
    for (size_t i = 0; i < cnt; ++i) {
        indices[i] = (int)g_prime_index_map[pf[i]];
        exps[i]    = ep[i];
    }
    int m = (int)cnt;

    uint64_t used[MAX_GROUPS];
    int used_cnt = 0;
    int found_cnt = 0;
    bool stop = false;

    dfs_index(0, exps, indices, m, used, used_cnt, &found_cnt, &stop);

    return (found_cnt == 1);
}

/* ------------------------------------------------------------- */
/* Main sequence function                                         */
/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    static seq_t  *seq      = NULL;
    static size_t seq_len   = 0;
    static size_t seq_cap   = 0;
    static uint64_t last_seen = 1;
    static bool init_done = false;

    if (!init_done) {
        init_primes();
        init_done = true;
    }

    while (seq_len < n) {
        if (has_unique_partition(last_seen)) {
            grow_seq_array(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)last_seen;
        }
        ++last_seen;
    }
    return seq[n - 1];
}
