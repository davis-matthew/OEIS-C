/*
 * A382080  Number of ways to partition the prime indices of n into sets with
 *          a common sum.
 * https://oeis.org/A382080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of factorizations of n into squarefree numbers >1 such
 *            that all factors have equal sums of prime indices. This is
 *            equivalent to counting partitions of the multiset of prime
 *            indices of n into groups with no repeated index and a common sum
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 10000          /* maximum n we will handle efficiently      */
#define MAX_PRIMES 1229      /* primes up to 10000                        */
#define MAX_SUBSETS 32       /* 2^5 > 32 (max distinct primes)           */

/* Global prime tables, built once. */
static int prime_list[MAX_PRIMES];
static int prime_index[MAX_N + 1];   /* 1‑based index of prime, 0 if composite */
static int num_primes = 0;
static bool primes_prepared = false;

/* Prepare prime tables. */
static void prepare_primes(void)
{
    if (primes_prepared) return;
    memset(prime_index, 0, sizeof(prime_index));
    bool is_prime[MAX_N + 1];
    for (int i = 0; i <= MAX_N; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= MAX_N; ++p) {
        if (is_prime[p]) {
            for (int k = p * p; k <= MAX_N; k += p) is_prime[k] = false;
        }
    }
    for (int i = 2; i <= MAX_N; ++i) {
        if (is_prime[i]) {
            prime_index[i] = ++num_primes;   /* 1‑based */
            prime_list[num_primes - 1] = i;
        }
    }
    primes_prepared = true;
}

/* Factor n into distinct primes with their exponents. */
static void factor_n(idx_t n, int *indices, int *exps, int *k)
{
    int cnt = 0;
    uint64_t temp = n;
    for (int pi = 0; pi < num_primes && (uint64_t)prime_list[pi] * prime_list[pi] <= temp; ++pi) {
        uint64_t p = prime_list[pi];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            indices[cnt] = prime_index[(int)p];
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        /* remaining prime factor > sqrt(n) */
        indices[cnt] = prime_index[(int)temp];
        exps[cnt] = 1;
        ++cnt;
    }
    *k = cnt;
}

/* Count partitions for given g and subset masks. */
static seq_t dfs(int idx, int rem_groups, int rem_e[5], const int *subset_masks,
                 int m, int k)
{
    if (idx == m) {
        if (rem_groups != 0) return 0;
        for (int i = 0; i < k; ++i) if (rem_e[i] != 0) return 0;
        return 1;
    }
    int mask = subset_masks[idx];
    /* Determine maximal x we can assign to this subset */
    int max_x = rem_groups;
    for (int i = 0; i < k; ++i) {
        if (mask & (1 << i)) {
            if (rem_e[i] < max_x) max_x = rem_e[i];
        }
    }
    seq_t total = 0;
    /* Try all possible counts for this subset */
    for (int x = 0; x <= max_x; ++x) {
        if (x) {
            for (int i = 0; i < k; ++i) {
                if (mask & (1 << i)) rem_e[i] -= x;
            }
        }
        total += dfs(idx + 1, rem_groups - x, rem_e, subset_masks, m, k);
        if (x) {
            for (int i = 0; i < k; ++i) {
                if (mask & (1 << i)) rem_e[i] += x;
            }
        }
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;  /* special case: empty product counts as 1 */

    prepare_primes();

    int indices[5];
    int exps[5];
    int k = 0;
    factor_n(n, indices, exps, &k);
    if (k == 0) return 0;  /* should not happen for n>=2 */

    /* Compute total sum of indices weighted by exponents */
    int totalSum = 0;
    int max_e = 0;
    int sum_e = 0;
    for (int i = 0; i < k; ++i) {
        totalSum += exps[i] * indices[i];
        if (exps[i] > max_e) max_e = exps[i];
        sum_e += exps[i];
    }

    /* Generate divisors of totalSum */
    int divisors[128];
    int dcnt = 0;
    for (int d = 1; d * (idx_t)d <= totalSum; ++d) {
        if (totalSum % d == 0) {
            divisors[dcnt++] = d;
            if (d != totalSum / d) divisors[dcnt++] = totalSum / d;
        }
    }

    seq_t result = 0;

    /* For each possible number of groups g */
    for (int di = 0; di < dcnt; ++di) {
        int g = divisors[di];
        if (g < max_e || g > sum_e) continue;  /* constraints impossible */
        int s = totalSum / g;                  /* sum per group */

        /* Gather all subsets of primes that sum to s */
        int subset_masks[MAX_SUBSETS];
        int m = 0;
        for (int mask = 1; mask < (1 << k); ++mask) {
            int sm = 0;
            for (int i = 0; i < k; ++i) if (mask & (1 << i)) sm += indices[i];
            if (sm == s) {
                subset_masks[m++] = mask;
            }
        }
        if (m == 0) continue;  /* no valid group shapes */

        int rem_e[5];
        memcpy(rem_e, exps, k * sizeof(int));
        result += dfs(0, g, rem_e, subset_masks, m, k);
    }

    /* Add the trivial factorization into n itself if n is squarefree >1 and
       satisfies the condition (each prime appears at most once). */
    /* For squarefree n with all exponents 1, we'll get g=1 case above. */
    return result;
}
