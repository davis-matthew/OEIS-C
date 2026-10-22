/*
 * A381634  Number of multisets that can be obtained by taking the sum of each
 *          block of a set multipartition (multiset of sets) of the prime
 *          indices of n with distinct block-sums.
 * https://oeis.org/A381634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct multisets of sums obtained from
 *            partitions of the multiset of prime indices of n (the index of a
 *            prime p is the number of primes ≤ p) with all block sums
 *            distinct
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A naive  enumeration is feasible because n ≤ 10000 and
 *  the number of prime indices in any n is at most 13. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Prime table and pi() implementation ---------- */
static uint32_t *prime_list = NULL;               /* list of primes */
static size_t   prime_cnt   = 0;                  /* number of primes */
static unsigned *pi_table   = NULL;               /* pi[i] = primes ≤ i */
static uint64_t  current_max = 0;                 /* prime list built up to this value */

/* Build a sieve up to at least max_needed. */
static void ensure_primes(uint64_t max_needed)
{
    if (current_max >= max_needed) return;

    uint64_t n = max_needed;
    /* Boolean sieve */
    uint8_t *sieve = (uint8_t *)calloc((size_t)n + 1, sizeof(uint8_t));
    if (!sieve) exit(1);
    for (uint64_t i = 2; i <= n; ++i) sieve[i] = 1;
    for (uint64_t i = 2; i * i <= n; ++i)
        if (sieve[i])
            for (uint64_t j = i * i; j <= n; j += i)
                sieve[j] = 0;

    /* Count primes and build list */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= n; ++i)
        if (sieve[i]) ++cnt;

    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) exit(1);
    size_t k = 0;
    for (uint64_t i = 2; i <= n; ++i)
        if (sieve[i]) new_primes[k++] = (uint32_t)i;

    /* Build pi table */
    unsigned *new_pi = (unsigned *)calloc((size_t)n + 1, sizeof(unsigned));
    if (!new_pi) exit(1);
    unsigned acc = 0;
    for (uint64_t i = 0; i <= n; ++i) {
        if (i >= 2 && sieve[i]) ++acc;
        new_pi[i] = acc;
    }

    /* Release old data */
    free(prime_list);
    free(pi_table);
    free(sieve);

    prime_list = new_primes;
    prime_cnt  = cnt;
    pi_table   = new_pi;
    current_max = n;
}

/* ---------- Data structures for the search ---------- */
typedef struct {
    int len;
    int sums[];          /* flex. array: sorted, distinct block sums */
} SumSet;

/* ---------- Recursive search for partitions ---------- */
static void dfs(uint32_t rem_mask, int depth,
                const int *S, int m,
                const int *sum_mask, const bool *valid,
                int *curr_sums,
                SumSet ***solutions, size_t *cnt, size_t *cap)
{
    if (rem_mask == 0) {
        /* Found a complete partition */
        int len = depth;
        int *tmp = (int *)malloc(len * sizeof(int));
        memcpy(tmp, curr_sums, len * sizeof(int));

        /* sort sums to get a canonical representation */
        for (int i = 1; i < len; ++i) {
            int key = tmp[i];
            int j = i - 1;
            while (j >= 0 && tmp[j] > key) {
                tmp[j + 1] = tmp[j];
                --j;
            }
            tmp[j + 1] = key;
        }

        /* Check if this multiset already exists */
        bool dup = false;
        for (size_t s = 0; s < *cnt; ++s) {
            SumSet *st = (*solutions)[s];
            if (st->len == len && memcmp(st->sums, tmp, len * sizeof(int)) == 0) {
                dup = true; break;
            }
        }

        if (!dup) {
            SumSet *ns = (SumSet *)malloc(sizeof(SumSet) + len * sizeof(int));
            ns->len = len;
            memcpy(ns->sums, tmp, len * sizeof(int));

            /* Ensure capacity for new solution */
            if (*cnt == *cap) {
                size_t new_cap = (*cap == 0) ? 4 : *cap * 2;
                SumSet **tmp_ptr = (SumSet **)realloc(*solutions,
                                            new_cap * sizeof(SumSet *));
                if (!tmp_ptr) exit(1);
                *solutions = tmp_ptr;
                *cap = new_cap;
            }
            (*solutions)[*cnt] = ns;
            ++(*cnt);
        }
        free(tmp);
        return;
    }

    /* Get lowest remaining element */
    int low = __builtin_ctz(rem_mask);
    uint32_t low_bit = 1U << low;

    /* Enumerate all submasks that contain this element */
    for (uint32_t sub = rem_mask; sub; sub = (sub - 1) & rem_mask) {
        if (!(sub & low_bit)) continue;
        if (!valid[sub]) continue;       /* block must have distinct values */

        int sum = sum_mask[sub];
        /* Ensure this sum hasn't been used already */
        bool conflict = false;
        for (int i = 0; i < depth; ++i) {
            if (curr_sums[i] == sum) { conflict = true; break; }
        }
        if (conflict) continue;

        curr_sums[depth] = sum;
        dfs(rem_mask ^ sub, depth + 1, S, m, sum_mask, valid,
            curr_sums, solutions, cnt, cap);
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* empty multiset gives one multiset */

    uint64_t nn = (uint64_t)n;
    ensure_primes(nn);             /* need primes up to n for factorization */

    /* Build multiset of prime indices of n (multiplicity counted) */
    int S[20];
    int m = 0;
    uint64_t tmp = nn;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            for (int e = 0; e < exp; ++e) {
                S[m++] = (int)pi_table[p];
            }
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        /* tmp fits in uint32_t because n ≤ 10000 in typical usage */
        uint32_t p = (uint32_t)tmp;
        S[m++] = (int)pi_table[p];
    }

    /* Precompute sum of subsets and validity (no duplicate values in block) */
    uint32_t total_masks = 1U << m;
    int *sum_mask = (int *)malloc(total_masks * sizeof(int));
    bool *valid   = (bool *)malloc(total_masks * sizeof(bool));
    if (!sum_mask || !valid) exit(1);

    sum_mask[0] = 0; valid[0] = true;
    for (uint32_t mask = 1; mask < total_masks; ++mask) {
        uint32_t low_bit = mask & -mask;
        int low = __builtin_ctz(mask);
        uint32_t prev = mask ^ low_bit;
        sum_mask[mask] = sum_mask[prev] + S[low];

        if (!valid[prev]) {
            valid[mask] = false;
        } else {
            /* check if S[low] occurs again in prev */
            bool dup = false;
            uint32_t bits = prev;
            while (bits) {
                int idx = __builtin_ctz(bits);
                if (S[idx] == S[low]) { dup = true; break; }
                bits &= bits - 1;
            }
            valid[mask] = !dup;
        }
    }

    /* Prepare DFS */
    SumSet **solutions = NULL;
    size_t sol_cnt = 0, sol_cap = 0;
    int curr_sums[20];

    dfs(total_masks - 1, 0, S, m, sum_mask, valid,
        curr_sums, &solutions, &sol_cnt, &sol_cap);

    /* Clean up */
    for (size_t i = 0; i < sol_cnt; ++i) free(solutions[i]);
    free(solutions);
    free(sum_mask);
    free(valid);

    return (seq_t)sol_cnt;
}
