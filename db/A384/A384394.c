/*
 * A384394  Number of proper ways to choose disjoint strict integer
 *          partitions, one of each conjugate prime index of n.
 * https://oeis.org/A384394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Number of proper ways to choose disjoint strict integer partitions, 
 * one of each conjugate prime index of n.
 * This implementation follows the definition from the A384394 entry:
 *   1.  Factor n.
 *   2.  Build the multiset of prime indices.
 *   3.  Compute its conjugate partition.
 *   4.  Count all tuples of strict partitions for the conjugate parts
 *       with the parts being pairwise distinct.  Subtract the all‑singleton
 *       choice if it is valid.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*                   Global data for primes                               */
/* --------------------------------------------------------------------- */

#define PRIME_LIMIT 20000
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Simple sieve of Eratosthenes */
static void init_primes(void)
{
    if (prime_list != NULL) return;
    bool *sieve = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!sieve) return; /* out of memory */
    for (int i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    /* Count primes */
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[idx++] = (uint32_t)i;
    free(sieve);
}

/* 1‑based index of a prime (prime(1) = 2) */
static size_t prime_index(uint32_t p)
{
    if (prime_list == NULL) init_primes();
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return lo + 1;   /* 1‑based */
}

/* --------------------------------------------------------------------- */
/*                Factorisation (used for n <= 10000)                     */
/* --------------------------------------------------------------------- */

static void factor_uint64(uint64_t n, uint32_t *pf, int *pe, size_t *cnt)
{
    if (prime_list == NULL) init_primes();
    size_t c = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pf[c] = p;
            pe[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pf[c] = (uint32_t)n;
        pe[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* --------------------------------------------------------------------- */
/*                   Helper for sorting indices                          */
/* --------------------------------------------------------------------- */

static int cmp_int_desc(const void *a, const void *b)
{
    return (*(int*)b - *(int*)a);
}

/* --------------------------------------------------------------------- */
/*               Recursive enumeration of partitions for a part           */
/* --------------------------------------------------------------------- */

static void dfs_recursive(int idx, int k, const int *t, bool *used, seq_t *total);

static void enumerate_partitions(int rem, int max_take, bool *used,
                                 int idx, int k, const int *t,
                                 seq_t *total)
{
    if (rem == 0) {
        dfs_recursive(idx + 1, k, t, used, total);
        return;
    }
    for (int x = max_take; x >= 1; --x) {
        if (used[x]) continue;          /* disjointness */
        used[x] = true;
        enumerate_partitions(rem - x, x - 1, used, idx, k, t, total);
        used[x] = false;
    }
}

static void dfs_recursive(int idx, int k, const int *t, bool *used, seq_t *total)
{
    if (idx == k) {
        (*total) += (seq_t)1;
        return;
    }
    int s = t[idx];
    enumerate_partitions(s, s, used, idx, k, t, total);
}

/* --------------------------------------------------------------------- */
/*                            Main function                               */
/* --------------------------------------------------------------------- */

#define MAX_USED_SIZE 2000   /* enough for all n <= 10000 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Factor n */
    uint32_t primes_factor[20];
    int exponents[20];
    size_t factor_cnt = 0;
    factor_uint64((uint64_t)n, primes_factor, exponents, &factor_cnt);

    if (factor_cnt == 0) return 0;   /* n == 1 */

    /* Build multiset of prime indices */
    int indices[200];               /* enough for 10000 */
    size_t idx_len = 0;
    for (size_t i = 0; i < factor_cnt; ++i) {
        size_t p_idx = prime_index(primes_factor[i]); /* 1‑based */
        for (int j = 0; j < exponents[i]; ++j)
            indices[idx_len++] = (int)p_idx;
    }
    if (idx_len == 0) return 0;

    /* Sort indices descending to form λ */
    qsort(indices, idx_len, sizeof(int), cmp_int_desc);

    int maxv = indices[0];          /* largest index value */
    if (maxv == 0) return 0;

    /* Compute conjugate partition λ' : parts array t[0..maxv-1] */
    int *t = (int*)malloc(maxv * sizeof(int));
    for (int i = 0; i < maxv; ++i) {
        int cnt = 0;
        for (size_t j = 0; j < idx_len; ++j) {
            if (indices[j] >= i + 1) ++cnt;
        }
        t[i] = cnt;                 /* λ'_ (i+1) */
    }

    /* Determine if all parts are distinct */
    bool all_distinct = true;
    for (int i = 1; i < maxv; ++i) {
        if (t[i] == t[i - 1]) { all_distinct = false; break; }
    }

    /* Count assignments */
    bool used[MAX_USED_SIZE];
    memset(used, 0, sizeof(used));

    seq_t total = 0;
    dfs_recursive(0, maxv, t, used, &total);

    if (all_distinct) total -= (seq_t)1;   /* subtract all‑singleton case if valid */

    free(t);
    return total;
}
