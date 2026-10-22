/*
 * A382858  Number of ways to permute a multiset whose multiplicities are the
 *          prime indices of n so that the run-lengths are all equal.
 * https://oeis.org/A382858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd_uint16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{L | g}  count_no_adj(L)
 *  where L runs over all common divisors of the list of prime indices of n,
 *  and count_no_adj(L) is the number of permutations of the multiset
 *  (see OEIS A382858) whose run‑lengths all equal L and no two equal
 *  runs are adjacent.
 *
 *  The multiset is constructed as follows:
 *      1. Let p₁…p_k be the distinct prime factors of n and let e_i
 *         be the exponents.  For each i, let i_p be the index of p_i
 *         in the list of primes 2,3,5,7,…  Add i_p to a list M e_i
 *         times.
 *      2. Write M = [m₁,…,m_r] (r = |M|).
 *      3. For each j (1≤j≤r) create a symbol j that appears m_j times.
 *         The resulting multiset has total size |M| = Σ m_j.
 *
 *  For a given L | g = gcd(m₁,…,m_r), let t_j = m_j / L.  The
 *  permutations that have all runs of length L correspond to
 *  arrangements of the R = Σ t_j runs, where t_j runs of symbol j
 *  are indistinguishable and no two consecutive runs have the same
 *  symbol.  The number of such arrangements is computed recursively
 *  with memoisation.
 *
 *  Complexity:  O(#states) where a state is defined by the remaining
 *  run counts and the previous symbol.  For n ≤ 10 000 this is
 *  easily under the time limit.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/gcd_uint16.h"

#define MAXN 10000
#define PRIME_CAP 1300          /* number of primes ≤ 10000 */

/*--- global prime data ---------------------------------------------------*/
static uint64_t prime_list[PRIME_CAP];
static uint16_t prime_index[MAXN + 1] = {0};   /* 0 for non‑prime, else 1‑based index */
static size_t prime_cnt = 0;
static bool primes_initialised = false;

static void init_primes(void)
{
    if (primes_initialised) return;
    bool is_prime[MAXN + 1];
    for (int i = 0; i <= MAXN; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= MAXN; ++p)
        if (is_prime[p]) {
            for (int k = p * p; k <= MAXN; k += p) is_prime[k] = false;
        }
    size_t idx = 0;
    for (int n = 2; n <= MAXN; ++n) if (is_prime[n]) {
        prime_list[idx] = n;
        prime_index[n] = (uint16_t)(idx + 1); /* 1‑based index */
        ++idx;
    }
    prime_cnt = idx;
    primes_initialised = true;
}

/*--- factorisation and construction of M --------------------------------*/
static void factor_and_build_M(idx_t n, uint16_t *M, size_t *r_out)
{
    init_primes();
    size_t r = 0;
    idx_t tmp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = (int)prime_list[i];
        if ((int64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            uint16_t idx = prime_index[p];
            for (int e = 0; e < exp; ++e) M[r++] = idx;
        }
    }
    if (tmp > 1) { /* tmp is prime */
        uint16_t idx = prime_index[(int)tmp];
        M[r++] = idx;
    }
    *r_out = r;
}

/*--- cache for DP --------------------------------------------------------*/
typedef struct {
    uint64_t key;
    seq_t val;
} CacheEntry;

static CacheEntry *make_cache(size_t cap)
{
    CacheEntry *c = (CacheEntry *)malloc(cap * sizeof(CacheEntry));
    for (size_t i = 0; i < cap; ++i) c[i].key = UINT64_MAX;
    return c;
}
static void free_cache(CacheEntry *c) { free(c); }

/*--- recursion with memoisation -----------------------------------------*/
static seq_t dfs(uint8_t *t, int prev, int r, CacheEntry *cache, size_t cap)
{
    /* encode state */
    uint64_t key = (uint64_t)(prev + 1);             /* 0 reserved for empty key */
    for (int i = 0; i < r; ++i) key |= ((uint64_t)t[i]) << (8 * (i + 1));
    /* lookup */
    size_t pos = key & (cap - 1);
    while (cache[pos].key != UINT64_MAX && cache[pos].key != key) pos = (pos + 1) & (cap - 1);
    if (cache[pos].key == key) return cache[pos].val;

    /* base case */
    int sum = 0;
    for (int i = 0; i < r; ++i) sum += t[i];
    if (sum == 0) return 1;

    seq_t total = 0;
    for (int i = 0; i < r; ++i) if (t[i] && i != prev) {
        --t[i];
        seq_t sub = dfs(t, i, r, cache, cap);
        total += sub;
        ++t[i];
    }
    /* store */
    cache[pos].key = key;
    cache[pos].val = total;
    return total;
}

/*--- main sequence function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of OEIS offset, safe return */

    if (n == 1) return 1;          /* a(1) = 1 */

    /* build multiplicity list M */
    uint16_t M[64];                /* r ≤ 13 for n ≤ 10 000, safe buffer */
    size_t r = 0;
    factor_and_build_M(n, M, &r);

    if (r == 0) return 1;          /* n = 1 case handled above */

    /* compute gcd of M */
    uint16_t g = M[0];
    for (size_t i = 1; i < r; ++i) g = gcd_uint16(g, M[i]);

    /* enumerate all divisors of g */
    uint16_t divisors[32];
    size_t dcnt = 0;
    for (uint16_t i = 1; i * i <= g; ++i) {
        if (g % i == 0) {
            divisors[dcnt++] = i;
            if (i != g / i) divisors[dcnt++] = g / i;
        }
    }

    seq_t total = 0;

    /* cache capacity – power of two > possible states */
    const size_t CACHE_CAP = 1 << 15; /* 32768, enough for r≤5 and small t_i */

    for (size_t d = 0; d < dcnt; ++d) {
        uint16_t L = divisors[d];

        /* compute t_i = M[i] / L */
        uint8_t t[64];
        bool ok = true;
        for (size_t i = 0; i < r; ++i) {
            if (M[i] % L != 0) { ok = false; break; } /* should not happen */
            t[i] = (uint8_t)(M[i] / L);
        }
        if (!ok) continue;

        /* special case r == 1 */
        if (r == 1) {
            if (t[0] == 1) total += 1;   /* only L == M[0] gives t[0]==1 */
            continue;
        }

        CacheEntry *cache = make_cache(CACHE_CAP);
        seq_t cur = dfs(t, -1, (int)r, cache, CACHE_CAP);
        total += cur;
        free_cache(cache);
    }

    return total;
}
