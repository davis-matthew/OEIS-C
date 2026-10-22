/*
 * A327012  Number of factorizations of n into factors > 1 whose dual is a
 *          (strict) antichain.
 * https://oeis.org/A327012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of unordered factorizations of n into ints > 1
 *            whose dual multiset system is a strict antichain
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_N 10000
#define MAX_FACTS 32
#define MAX_PRIMES 8

static uint32_t spf[MAX_N + 1];
static bool spf_inited = false;

static uint32_t prime_of_n[MAX_PRIMES];
static int prime_count_n;

static void init_spf(void) {
    if (spf_inited) return;
    for (int i = 0; i <= MAX_N; ++i) spf[i] = 0;
    spf[0] = 0; spf[1] = 1;
    for (int i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((long long)i * i <= MAX_N) {
                for (int j = i * i; j <= MAX_N; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_inited = true;
}

static void factor_into_primes(idx_t n) {
    prime_count_n = 0;
    idx_t m = n;
    while (m > 1) {
        uint32_t p = spf[m];
        while (m % p == 0) m /= p;
        prime_of_n[prime_count_n++] = p;
    }
}

static idx_t facts_arr[MAX_FACTS];

static bool is_antichain(const idx_t *facts, size_t k) {
    if (k == 0) return true; /* for n == 1, handled separately */
    static int counts[MAX_PRIMES][MAX_FACTS];
    for (int i = 0; i < prime_count_n; ++i)
        for (size_t t = 0; t < k; ++t)
            counts[i][t] = 0;

    for (size_t fi = 0; fi < k; ++fi) {
        idx_t f = facts[fi];
        while (f > 1) {
            uint32_t p = spf[f];
            int cnt = 0;
            while (f % p == 0) {
                f /= p;
                ++cnt;
            }
            int pi = -1;
            for (int t = 0; t < prime_count_n; ++t)
                if (prime_of_n[t] == p) { pi = t; break; }
            if (pi >= 0) counts[pi][fi] += cnt;
        }
    }

    for (int i = 0; i < prime_count_n; ++i) {
        for (int j = i + 1; j < prime_count_n; ++j) {
            bool sub01 = true, sub10 = true;
            for (size_t t = 0; t < k; ++t) {
                if (counts[i][t] > counts[j][t]) sub01 = false;
                if (counts[j][t] > counts[i][t]) sub10 = false;
            }
            if (sub01 || sub10) return false;
        }
    }
    return true;
}

static void dfs(idx_t rem, idx_t start, size_t depth, seq_t *cnt) {
    if (rem == 1) {
        if (is_antichain(facts_arr, depth))
            ++(*cnt);
        return;
    }
    for (idx_t f = start; f <= rem; ++f) {
        if (rem % f == 0) {
            facts_arr[depth] = f;
            dfs(rem / f, f, depth + 1, cnt);
        }
    }
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    init_spf();
    if (n > MAX_N) return (seq_t)0;  /* problem guarantees n <= 10000 */

    factor_into_primes(n);
    seq_t res = 0;
    dfs(n, 2, 0, &res);
    return res;
}
