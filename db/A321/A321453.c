/*
 * A321453  Numbers that cannot be factored into two or more factors all
 *          having the same sum of prime indices.
 * https://oeis.org/A321453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321453:  Numbers that cannot be factored into two or more
   factors all having the same sum of prime indices.
   Algorithm: for each integer m, factorize m, build the multiset of prime
   indices (each prime factor contributes its ordinal index as many times
   as its exponent).  If this multiset can be partitioned into k (k≥2)
   non‑empty subsets each having identical total index sum, then m is
   excluded; otherwise it belongs to the sequence.  The routine builds
   the sequence incrementally, memoising results between calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* --- backtracking to test equal‑sum partition ----------------------------- */
static bool dfs_partition(int idx, int M, int k, int target,
                          const int *sorted, int *sums)
{
    if (idx == M) {
        /* all elements placed – all groups must reach target */
        for (int i = 0; i < k; ++i)
            if (sums[i] != target) return false;
        return true;
    }
    int val = sorted[idx];
    for (int i = 0; i < k; ++i) {
        if (sums[i] + val > target) continue;
        if (i > 0 && sums[i] == sums[i - 1]) continue; /* avoid symmetry */
        sums[i] += val;
        if (dfs_partition(idx + 1, M, k, target, sorted, sums))
            return true;
        sums[i] -= val;
    }
    return false;
}

/* --- check whether multiset of indices can be partitioned ----------- */
static bool has_equal_sum_partition(const int *indices, int M)
{
    if (M <= 1) return false;          /* cannot form ≥2 non‑empty groups */

    int total = 0;
    for (int i = 0; i < M; ++i) total += indices[i];

    int sorted[64];
    for (int i = 0; i < M; ++i) sorted[i] = indices[i];

    /* simple bubble sort descending */
    for (int i = 0; i < M - 1; ++i)
        for (int j = i + 1; j < M; ++j)
            if (sorted[i] < sorted[j]) {
                int t = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = t;
            }

    for (int k = 2; k <= M; ++k) {
        if (total % k != 0) continue;
        int target = total / k;
        if (sorted[0] > target) continue;       /* largest element too big */

        int sums[64];
        memset(sums, 0, k * sizeof(int));

        if (dfs_partition(0, M, k, target, sorted, sums))
            return true;
    }
    return false;
}

static const size_t PRIME_LIMIT = 2000000;          /* sieve limit   */
static uint32_t *prime_list = NULL;                /* primes         */
static size_t   prime_cnt  = 0;                    /* number of primes  */
static uint32_t *prime_index_of = NULL;            /* index of prime  */
static bool primes_inited = false;

/* --- prime sieve and helper ------------------------------------------------ */
static void init_primes(void)
{
    if (primes_inited) return;

    /* sieve */
    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) exit(1);
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }

    /* count primes */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_cnt++;

    /* store primes */
    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    prime_index_of = (uint32_t *)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    size_t pos = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            prime_list[pos] = (uint32_t)i;
            prime_index_of[i] = (uint32_t)(pos + 1);   /* 1‑based index */
            ++pos;
        }
    }
    primes_inited = true;
    free(sieve);
}

/* --- true if number is part of the sequence ------------------------------ */
static bool is_good(uint64_t n)
{
    if (n == 1) return true;

    init_primes();

    int indices[64];
    int M = 0;
    uint64_t tmp = n;

    for (size_t pi = 0; pi < prime_cnt &&
         (uint64_t)prime_list[pi] * prime_list[pi] <= tmp; ++pi) {
        uint32_t p = prime_list[pi];
        while (tmp % p == 0) {
            indices[M++] = prime_index_of[p];
            tmp /= p;
        }
    }
    if (tmp > 1) {
        /* remaining prime factor > sqrt(n) */
        if (tmp <= PRIME_LIMIT) {
            indices[M++] = prime_index_of[(uint32_t)tmp];
        } else {
            /* unlikely for our limits */
            return true;
        }
    }

    return !has_equal_sum_partition(indices, M);
}

/* --- sequence construction ------------------------------------------------- */
static seq_t *seq_cache = NULL;   /* cached a(i) values */
static size_t seq_len  = 0;       /* number of cached terms   */
static idx_t  last_num  = 0;      /* last integer checked     */

seq_t a(idx_t n)
{
    /* Handle cached results */
    while (seq_len < (size_t)n) {
        ++last_num;
        if (is_good((uint64_t)last_num)) {
            seq_cache = mem_resize(seq_t, seq_cache, (seq_len + 1));
            seq_cache[seq_len] = (seq_t)last_num;
            seq_len++;
        }
    }
    return seq_cache[n - 1];
}
