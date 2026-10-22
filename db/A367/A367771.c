/*
 * A367771  Number of ways to choose a different prime index of each prime
 *          index of 2n + 1.
 * https://oeis.org/A367771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ways to choose a different prime index of each prime
 *            index of 2n + 1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Constants */
#define MAX_LIMIT   40010          /* enough for 2*n+1 with n up to ~10000 */
#define MAX_PRIMES  5000           /* maximum number of primes up to MAX_LIMIT */
#define MAX_OPTIONS 8              /* maximum distinct prime‐index values for a number ≤ MAX_PRIMES */
#define MAX_GROUPS  30             /* maximum number of prime‑index–occurrences in 2*n+1 */

/* Global data structures */
static bool initialized = false;
static uint32_t spf[MAX_LIMIT + 1];          /* smallest prime factor */
static uint32_t prime_index[MAX_LIMIT + 1];   /* 0 for non‑primes, otherwise 1‑based index of prime */
static uint32_t primes[MAX_PRIMES];
static size_t  primes_cnt = 0;

static int opt_num[MAX_PRIMES + 1];           /* number of distinct values for each x (prime index) */
static int opt_val[MAX_PRIMES + 1][MAX_OPTIONS];
static int opt_c [MAX_PRIMES + 1][MAX_OPTIONS];

static int groupOptIdx[MAX_GROUPS];
static int  totalGroups;
static bool usedVals[MAX_PRIMES + 1];
static seq_t result_global;

/* ------------------------------------------------------------------ */
/* Initialization: compute primes, spf, prime_index, and option tables */

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* sieve for spf and primes */
    for (uint32_t i = 0; i <= MAX_LIMIT; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt < MAX_PRIMES) primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* prime_index: 0 for non‑prime, otherwise 1‑based index */
    for (uint32_t i = 0; i <= MAX_LIMIT; ++i) prime_index[i] = 0;
    for (size_t k = 0; k < primes_cnt; ++k) prime_index[primes[k]] = (uint32_t)(k + 1);

    /* precompute prime‑index options for all numbers up to primes_cnt */
    for (int x = 1; x <= (int)primes_cnt; ++x) {
        opt_num[x] = 0;   /* default empty */
        if (x == 1) continue;  /* 1 has no prime factors */

        int temp = x;
        int idxs[MAX_OPTIONS];
        int cnts[MAX_OPTIONS];
        int num = 0;

        while (temp > 1) {
            int p = (int)spf[temp];
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }

            int idx = (int)prime_index[p];  /* prime index of p */
            int j = 0;
            while (j < num && idxs[j] != idx) ++j;
            if (j == num) {               /* new value */
                idxs[num] = idx;
                cnts[num] = e;
                ++num;
            } else {                      /* existing value */
                cnts[j] += e;
            }
        }

        opt_num[x] = num;
        for (int j = 0; j < num; ++j) {
            opt_val[x][j] = idxs[j];
            opt_c [x][j] = cnts[j];
        }
    }
}

/* ------------------------------------------------------------------ */
/* Recursive backtracking to count assignments */

static void dfs(int depth, seq_t prod)
{
    if (depth == totalGroups) {
        result_global += prod;
        return;
    }

    int x = groupOptIdx[depth];
    int num = opt_num[x];
    for (int j = 0; j < num; ++j) {
        int v = opt_val[x][j];
        if (!usedVals[v]) {
            usedVals[v] = true;
            dfs(depth + 1, prod * (seq_t)opt_c[x][j]);
            usedVals[v] = false;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */

seq_t a(idx_t n)
{
    init();

    /* compute m = 2n + 1 */
    uint32_t m = (uint32_t)(2ULL * (uint64_t)n + 1ULL);

    if (m <= 1) return (seq_t)1;  /* n == 0 */

    /* factorize m, building the list of group prime indices */
    totalGroups = 0;
    uint32_t temp = m;
    while (temp > 1) {
        uint32_t p = spf[temp];
        int e = 0;
        while (temp % p == 0) { temp /= p; ++e; }

        /* prime index of p */
        int idx = (int)prime_index[p];
        for (int t = 0; t < e; ++t) {
            if (totalGroups < MAX_GROUPS) groupOptIdx[totalGroups++] = idx;
        }
    }

    if (totalGroups == 0) return (seq_t)1;

    memset(usedVals, 0, sizeof(usedVals));
    result_global = 0;
    dfs(0, (seq_t)1);
    return result_global;
}
