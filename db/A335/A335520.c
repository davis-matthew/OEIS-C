/*
 * A335520  Number of (1,2,3)-matching permutations of the prime indices of n.
 * https://oeis.org/A335520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of the multiset of prime indices
 *            of n that * contain an increasing subsequence of length 3. * The
 *            multiset is formed from the exponents in the prime factorization
 *            of n, * each exponent corresponding to the repetition of the
 *            prime’s index. * a(n) = total_permutations...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static seq_t fact[21];
static bool fact_computed = false;
static void init_fact(void) {
    if (fact_computed) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i <= 20; ++i) fact[i] = fact[i-1] * (seq_t)i;
    fact_computed = true;
}

/* Cache entry for previously seen exponent vectors */
typedef struct {
    uint8_t k;
    uint8_t exps[15];
    seq_t avoid;
    bool computed;
} CacheEntry;
static CacheEntry cache[20000];
static size_t cache_cnt = 0;

static CacheEntry *get_cache(int k, uint8_t *exps) {
    for (size_t i = 0; i < cache_cnt; ++i) {
        if (cache[i].computed && cache[i].k == k &&
            memcmp(cache[i].exps, exps, k) == 0) return &cache[i];
    }
    CacheEntry *e = &cache[cache_cnt++];
    e->k = k;
    memcpy(e->exps, exps, k);
    e->computed = false;
    return e;
}

/* Recursive DFS: count permutations that keep LIS length ≤ 2 */
static seq_t dfs_rec(uint8_t *rem, int *tails, int max_len,
                     int remaining, int k) {
    if (remaining == 0) return (seq_t)1;
    seq_t cnt = 0;
    for (int i = 0; i < k; ++i) {
        if (rem[i] == 0) continue;
        rem[i]--;
        int v = i + 1;                     /* value labels 1..k */
        int new_tails[16];
        memcpy(new_tails, tails, sizeof(int) * k);
        int pos = 0;
        while (pos < max_len && new_tails[pos] < v) pos++;
        new_tails[pos] = v;
        int new_max = max_len;
        if (pos == max_len) new_max = pos + 1;
        if (new_max < 3) {
            cnt += dfs_rec(rem, new_tails, new_max, remaining - 1, k);
        }
        rem[i]++;   /* backtrack */
    }
    return cnt;
}

static seq_t compute_avoid(int k, uint8_t *exps) {
    CacheEntry *e = get_cache(k, exps);
    if (e->computed) return e->avoid;
    uint8_t rem[15];
    memcpy(rem, exps, k);
    int tails[16];
    memset(tails, 0, sizeof(tails));
    int N = 0;
    for (int i = 0; i < k; ++i) N += exps[i];
    seq_t avoid = dfs_rec(rem, tails, 0, N, k);
    e->avoid = avoid;
    e->computed = true;
    return avoid;
}

/* Prime factorization of n.  n ≤ 10000, trial division is fast. */
static void factorize_n(idx_t idx_n, uint8_t *exps, int *k_out) {
    uint64_t n = (uint64_t)idx_n;
    int k = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                ++cnt;
                n /= p;
            }
            exps[k++] = (uint8_t)cnt;
        }
    }
    if (n > 1) exps[k++] = 1;  /* remaining prime */
    *k_out = k;
}

seq_t a(idx_t n) {
    init_fact();
    uint8_t exps[15];
    int k;
    factorize_n(n, exps, &k);
    /* total permutations: N! / product(e_i!) */
    int N = 0;
    for (int i = 0; i < k; ++i) N += exps[i];
    seq_t total = fact[N];
    for (int i = 0; i < k; ++i) total /= fact[exps[i]];
    seq_t avoid = compute_avoid(k, exps);
    return total - avoid;
}
