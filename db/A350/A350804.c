/*
 * A350804  Numbers k with exactly one partition into two parts (s,t), s<=t,
 *          such that t | s*k.
 * https://oeis.org/A350804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k with exactly one divisor d of k^2 satisfying k/2 <= d
 *            < k; computed by searching sequentially and counting divisors of
 *            k^2
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spf = NULL;
static idx_t spf_limit = 0;

static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static idx_t last_k     = 0;

/* initialise smallest prime factor table up to LIMIT */
static void init_spf(idx_t limit) {
    spf_limit = limit;
    spf = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((unsigned long long)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* extend SPF table so that it covers at least N */
static void ensure_spf(idx_t needed) {
    if (spf_limit >= needed) return;
    idx_t new_limit = spf_limit ? spf_limit * 2 : 2;
    while (new_limit < needed) new_limit *= 2;
    free(spf);
    init_spf(new_limit);
}

/* recursive divisor enumeration */
static void count_rec(const uint32_t *primes, const uint8_t *exps, int idx, int pf_cnt,
                      unsigned long long cur, unsigned long long lim, unsigned long long *out) {
    if (idx == pf_cnt) {
        if (cur <= lim) (*out)++;
        return;
    }
    unsigned long long val = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        if (val > lim) break;
        count_rec(primes, exps, idx + 1, pf_cnt, val, lim, out);
        if (e < exps[idx]) val *= primes[idx];
    }
}

/* count divisors of k^2 that are <= LIM */
static unsigned long long count_divisors_le_k(idx_t k, unsigned long long lim) {
    uint32_t primes[20];
    uint8_t  exps[20];
    int pf_cnt = 0;
    idx_t x = k;
    while (x > 1) {
        uint32_t p = spf[x];
        unsigned cntp = 0;
        while (x % p == 0) { x /= p; ++cntp; }
        primes[pf_cnt] = p;
        exps[pf_cnt]  = (uint8_t)(cntp * 2);   /* exponent in k^2 */
        ++pf_cnt;
    }
    unsigned long long res = 0;
    count_rec(primes, exps, 0, pf_cnt, 1ULL, lim, &res);
    return res;
}

/* return true iff k has exactly one divisor d of k^2 with k/2 <= d < k */
static bool qualifies(idx_t k) {
    unsigned long long lt_k   = count_divisors_le_k(k, (unsigned long long)(k - 1));
    unsigned long long lt_half = count_divisors_le_k(k, (unsigned long long)((k - 1) / 2));
    return (lt_k - lt_half) == 1;
}

/* OEIS sequence A350804 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined for index 0 */
    if (!spf) ensure_spf(100000);          /* initial SPF table */
    while (seq_len < (size_t)n) {
        ++last_k;
        ensure_spf(last_k);                 /* extend SPF if needed */
        if (qualifies(last_k)) {
            seq_cache = (seq_t*)realloc(seq_cache, (seq_len + 1) * sizeof(seq_t));
            if (!seq_cache) exit(1);
            seq_cache[seq_len] = (seq_t)last_k;
            ++seq_len;
        }
    }
    return seq_cache[n - 1];
}
