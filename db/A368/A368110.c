/*
 * A368110  Numbers of which it is possible to choose a different divisor of
 *          each prime index.
 * https://oeis.org/A368110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A368110: numbers for which a distinct divisor can be chosen
   for each occurrence of each prime index.  The function uses Hall's
   marriage theorem and a bipartite matching algorithm.  It is
   thread‑safe only for a single thread due to the use of static state. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Parameters ---------- */
#define SIEVE_LIMIT 200000          /* maximum value in sieve */
#define MAX_OCC    64               /* maximum total prime‑index occurrences */
#define MAX_DIV    512              /* maximum distinct divisor values */

/* ---------- Prime sieve data ---------- */
static bool is_prime_arr[SIEVE_LIMIT + 1];
static uint32_t primes_arr[SIEVE_LIMIT];
static size_t primes_cnt = 0;
static uint32_t prime_idx_of_val[SIEVE_LIMIT + 1];

/* ---------- Matching data ---------- */
static int neigh[MAX_OCC][MAX_DIV];
static int neigh_cnt[MAX_OCC];
static int match_to_occ[MAX_DIV];
static unsigned char seen[MAX_DIV];

/* ---------- Cached sequence values ---------- */
static seq_t *cached_vals = NULL;
static size_t cached_len = 0;
static size_t cached_cap = 0;
static idx_t candidate = 1;

/* ---------- Helper: generate primes up to SIEVE_LIMIT ---------- */
static void build_primes(void)
{
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime_arr[i] = true;
    is_prime_arr[0] = is_prime_arr[1] = false;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_arr[j] = false;
        }
    }
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime_arr[i]) {
            primes_arr[primes_cnt++] = (uint32_t)i;
            prime_idx_of_val[i] = (uint32_t)primes_cnt;   /* 1‑based index */
        } else {
            prime_idx_of_val[i] = 0;
        }
    }
}

/* ---------- Helper: prime factorisation (small primes only) ---------- */
static void factor_uint64(uint64_t n, uint32_t *pr, int *ex, int *cnt)
{
    int c = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes_arr[i] * primes_arr[i] <= n; ++i) {
        if (n % primes_arr[i] == 0) {
            int e = 0;
            while (n % primes_arr[i] == 0) {
                n /= primes_arr[i];
                ++e;
            }
            pr[c]   = primes_arr[i];
            ex[c++] = e;
        }
    }
    if (n > 1) {
        /* n itself is prime (>SIEVE_LIMIT) */
        pr[c]   = (uint32_t)n;
        ex[c++] = 1;
    }
    *cnt = c;
}

/* ---------- Helper: collect divisors of an integer <= SIEVE_LIMIT ---------- */
static void get_divisors(uint32_t m, uint32_t *divs, int *cnt)
{
    int c = 0;
    for (uint32_t d = 1; (uint64_t)d * d <= m; ++d) {
        if (m % d == 0) {
            divs[c++] = d;
            if (d * d != m)
                divs[c++] = m / d;
        }
    }
    *cnt = c;
}

/* ---------- DFS for bipartite matching ---------- */
static bool dfs_occ(int occ_idx)
{
    for (int k = 0; k < neigh_cnt[occ_idx]; ++k) {
        int d = neigh[occ_idx][k];
        if (seen[d]) continue;
        seen[d] = 1;
        if (match_to_occ[d] == -1 || dfs_occ(match_to_occ[d])) {
            match_to_occ[d] = occ_idx;
            return true;
        }
    }
    return false;
}

/* ---------- Check if n is a term of the sequence ---------- */
static bool is_good(uint64_t n)
{
    if (n == 1) return true;

    uint32_t primes_small[16];
    int   exps[16];
    int   fcnt, occ_cnt = 0;

    factor_uint64(n, primes_small, exps, &fcnt);

    /* Record each occurrence of prime index (repeated for multiplicity) */
    uint32_t occ_idx[MAX_OCC];
    for (int i = 0; i < fcnt; ++i) {
        uint32_t p = primes_small[i];
        uint32_t idx;
        if (p <= SIEVE_LIMIT)
            idx = prime_idx_of_val[p];
        else {
            /* count primes up to p (p is prime > SIEVE_LIMIT) */
            idx = 0;
            for (size_t j = 2; j <= p; ++j)
                if (is_prime_arr[j]) ++idx;
        }
        for (int t = 0; t < exps[i]; ++t) {
            if (occ_cnt < MAX_OCC)
                occ_idx[occ_cnt++] = idx;
        }
    }

    /* Collect all distinct divisors of the indices */
    uint32_t div_vals[MAX_DIV];
    int div_cnt = 0;
    for (int i = 0; i < occ_cnt; ++i) {
        uint32_t m = occ_idx[i];
        uint32_t sub_divs[64];
        int sub_cnt;
        get_divisors(m, sub_divs, &sub_cnt);
        for (int j = 0; j < sub_cnt; ++j) {
            uint32_t val = sub_divs[j];
            bool found = false;
            for (int k = 0; k < div_cnt; ++k)
                if (div_vals[k] == val) { found = true; break; }
            if (!found && div_cnt < MAX_DIV)
                div_vals[div_cnt++] = val;
        }
    }

    /* Build adjacency lists from occurrences to divisor indices */
    for (int i = 0; i < occ_cnt; ++i) {
        uint32_t m = occ_idx[i];
        uint32_t sub_divs[64];
        int sub_cnt;
        get_divisors(m, sub_divs, &sub_cnt);
        int pos = 0;
        for (int j = 0; j < sub_cnt; ++j) {
            uint32_t val = sub_divs[j];
            for (int k = 0; k < div_cnt; ++k)
                if (div_vals[k] == val) {
                    neigh[i][pos++] = k;
                    break;
                }
        }
        neigh_cnt[i] = pos;
        if (pos == 0) return false;   /* no divisor available -> fail */
    }

    /* Matching */
    for (int d = 0; d < div_cnt; ++d)
        match_to_occ[d] = -1;

    int matched = 0;
    for (int i = 0; i < occ_cnt; ++i) {
        memset(seen, 0, div_cnt);
        if (dfs_occ(i))
            ++matched;
        else
            break;
    }
    return matched == occ_cnt;
}

/* ---------- Append value to cached sequence ---------- */
static void add_value(seq_t v)
{
    if (cached_len == cached_cap) {
        cached_cap = (cached_cap == 0) ? 16 : cached_cap * 2;
        cached_vals = (seq_t *)realloc(cached_vals, cached_cap * sizeof(seq_t));
    }
    cached_vals[cached_len++] = v;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static bool primed = false;
    if (!primed) {
        build_primes();
        primed = true;
    }

    while (cached_len < n) {
        if (is_good((uint64_t)candidate))
            add_value((seq_t)candidate);
        ++candidate;
    }
    return cached_vals[n - 1];
}
