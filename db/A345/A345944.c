/*
 * A345944  Numbers k such that A002034(d) < A002034(k) for all the proper
 *          divisors d of k.
 * https://oeis.org/A345944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that A002034(d) < A002034(k) for all proper
 *            divisors d of k
 * Range:     u8 n <= 66, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   check_record_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/check_record_recursive.h"

/* static data */
static uint32_t *spf = NULL;          /* smallest prime factor table */
static size_t spf_limit = 0;          /* current maximal n for SPF */

static seq_t *S_cache = NULL;         /* cache of S(n) values */
static size_t S_cache_cap = 0;        /* capacity of S_cache */

static seq_t *seq_vals = NULL;        /* sequence of record numbers */
static size_t seq_cap = 0;            /* capacity of seq_vals */
static size_t seq_len = 0;            /* number of terms computed */

static size_t last_checked = 0;       /* last integer tested for being a record */

/* build SPF table up to n (inclusive) */
static void build_spf_upto(size_t n) {
    uint32_t *new_spf = (uint32_t *)realloc(spf, (n + 1) * sizeof(uint32_t));
    spf = new_spf;
    spf_limit = n;
    for (size_t i = 0; i <= n; ++i) spf[i] = 0;

    for (size_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            uint64_t start = (uint64_t)i * i;
            if (start <= n) {
                for (size_t j = (size_t)start; j <= n; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* ensure SPF table covers at least 'needed' */
static void ensure_spf_up_to(size_t needed) {
    if (needed <= spf_limit) return;
    build_spf_upto(needed);
}

/* factorise n into primes and exponents; arrays given must be large enough */
static void factorise_uint64(uint64_t n, uint32_t *pr_out, uint32_t *exp_out, size_t *cnt_out) {
    size_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        pr_out[cnt] = p;
        exp_out[cnt] = e;
        ++cnt;
    }
    *cnt_out = cnt;
}

/* Legendre's formula: exponent of prime p in m! */
static unsigned int legendre(unsigned int m, unsigned int p) {
    unsigned int cnt = 0;
    while (m) {
        m /= p;
        cnt += m;
    }
    return cnt;
}

/* minimal m such that legendre(m,p) >= e */
static unsigned int min_m_for_prime_exp(unsigned int p, unsigned int e, unsigned int n) {
    unsigned int low = 1, high = n;
    while (low < high) {
        unsigned int mid = low + (high - low) / 2;
        if (legendre(mid, p) >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* compute S(n) */
static seq_t S_of_uint64(uint64_t n) {
    if (n <= 1) return (seq_t)1;
    uint32_t pr[64], exp[64];
    size_t cnt;
    factorise_uint64(n, pr, exp, &cnt);
    unsigned int m_max = 0;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned int m = min_m_for_prime_exp(pr[i], exp[i], (unsigned int)n);
        if (m > m_max) m_max = m;
    }
    return (seq_t)m_max;
}

/* ensure capacity of S_cache */
static void ensure_S_cache(size_t need) {
    if (need < S_cache_cap) return;
    size_t new_cap = S_cache_cap ? S_cache_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(S_cache, new_cap * sizeof(seq_t));
    S_cache = tmp;
    S_cache_cap = new_cap;
}

static bool is_record_candidate(uint64_t n, seq_t S_n, seq_t *S_cache) {
    uint32_t pr[64], exp[64];
    size_t cnt;
    factorise_uint64(n, pr, exp, &cnt);
    return check_record_recursive(1, 0, cnt, pr, exp, S_n, n, S_cache);
}

/* ensure capacity of seq_vals */
static void ensure_seq_vals(size_t need) {
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_vals = tmp;
    seq_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */

    if (seq_vals == NULL) {        /* initialize static state on first call */
        spf = NULL;
        spf_limit = 0;
        S_cache = NULL;
        S_cache_cap = 0;
        seq_vals = NULL;
        seq_cap = 0;
        seq_len = 0;
        last_checked = 0;
    }

    /* produce more terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        ++last_checked;
        ensure_spf_up_to(last_checked);

        seq_t S_k = S_of_uint64(last_checked);
        if (last_checked >= S_cache_cap) ensure_S_cache(last_checked + 1);
        S_cache[last_checked] = S_k;

        if (is_record_candidate(last_checked, S_k, S_cache)) {
            ensure_seq_vals(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq_vals[n - 1];
}
