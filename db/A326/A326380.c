/*
 * A326380  Numbers m such that beta(m) = tau(m)/2 where beta(m) is the number
 *          of Brazilian representations of m and tau(m) is the number of
 *          divisors of m.
 * https://oeis.org/A326380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer m (m ≥ 7) for which * τ(m) = 2·β(m),
 *            where τ(m) is the number of divisors of m * and β(m) is the
 *            number of Brazilian representations of m. * A Brazilian
 *            representation is a repdigit in a base b (2 ≤ b ≤ m‑2) * with at
 *            least two digits: * * m = d · (bⁿ − 1)/(b − 1) (d ≥ 1,...
 * Range:     u8 n <= 34, u16 n <= 2166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Utility: integer square root (floor) for 64‑bit unsigned values   */
/* ------------------------------------------------------------------ */
static idx_t isqrt_u64(idx_t n) {
    idx_t low = 0;
    idx_t high = (idx_t)1ULL << 32;   /* > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) low = mid; else high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/*  Prime sieve up to a fixed limit                                    */
/* ------------------------------------------------------------------ */
static const uint64_t PRIME_SIEVE_LIMIT = 2000000ULL;   /* 2×10⁶  */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* generate a list of primes up to PRIME_SIEVE_LIMIT */
static void init_prime_list(void) {
    if (prime_list != NULL) return;
    bool *is_composite = (bool *)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_composite) return;
    for (uint64_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count == 0) prime_list = NULL;
            if (!prime_list) prime_list = (uint64_t *)malloc(150000 * sizeof(uint64_t)); /* big enough for 2 M */
            prime_list[prime_count++] = i;
            if (i * i <= PRIME_SIEVE_LIMIT) {
                for (uint64_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ------------------------------------------------------------------ */
/*  Factorisation: compute distinct primes and exponents of n         */
/* ------------------------------------------------------------------ */
static void factorize(uint64_t n, uint64_t *pr, int *exp, int *cnt) {
    *cnt = 0;
    for (size_t i = 0; i < prime_count && (__int128)prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pr[(*cnt)] = p;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {   /* remaining prime */
        pr[(*cnt)] = n;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/*  Recursively generate all divisors of m from its factorisation     */
/* ------------------------------------------------------------------ */
static void gen_divisors_rec(const uint64_t *pr, const int *exp, int idx,
                             uint64_t cur, uint64_t m,
                             uint64_t max_b, uint64_t *beta_out) {
    if (idx == 0) {
        uint64_t R = cur;
        if (R > 1 && R > max_b + 1) {
            uint64_t b_val = R - 1;
            if (b_val <= m - 2) {
                uint64_t d = m / R;
                if (d < b_val) (*beta_out)++;
            }
        }
        return;
    }
    int emax = exp[idx - 1];
    uint64_t p = pr[idx - 1];
    uint64_t val = 1;
    for (int e = 0; e <= emax; ++e) {
        gen_divisors_rec(pr, exp, idx - 1, cur * val, m, max_b, beta_out);
        val *= p;
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence storage (dynamic array)                                   */
/* ------------------------------------------------------------------ */
static seq_t *seq_buf = NULL;
static idx_t seq_len = 0;
static idx_t seq_cap = 0;

static void seq_append(seq_t val) {
    if (seq_len == seq_cap) {
        idx_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
        seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_buf[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Main sequence generator: a(n)                                      */
/* ------------------------------------------------------------------ */
static idx_t last_checked = 6;   /* start check from 7 */

seq_t a(idx_t n) {
    if (n <= 0) return 0;   /* undefined but safe */

    /* initialise prime list once */
    init_prime_list();

    while (seq_len < n) {
        ++last_checked;
        /* Ignore small numbers (<7) – the first term is 7 */
        if (last_checked < 7) continue;

        uint64_t m = last_checked;
        /* factorisation */
        uint64_t primes_local[16];
        int exp_local[16];
        int cnt = 0;
        factorize(m, primes_local, exp_local, &cnt);

        /* compute τ(m) */
        uint64_t tau = 1;
        for (int i = 0; i < cnt; ++i) tau *= (uint64_t)(exp_local[i] + 1);

        /* compute β(m) */
        uint64_t beta = 0;
        /* upper bound for base to keep base < m-1 */
        uint64_t max_b = isqrt_u64(m);
        if (max_b > m - 2) max_b = m - 2;
        if (max_b < 2) max_b = 2; /* for m=7, max_b=5, but we need at least 2 */

        /* enumerate bases up to max_b */
        for (uint64_t b = 2; b <= max_b; ++b) {
            __int128 R = (__int128)b + 1;   /* start with k = 2 */
            while (R <= m) {
                if (m % (uint64_t)R == 0) {
                    uint64_t d = m / (uint64_t)R;
                    if (d < b) ++beta;
                }
                R = R * b + 1;   /* next k */
            }
        }

        /* length‑2 representations with bases > max_b but < m-1
         * handled by divisor enumeration */
        gen_divisors_rec(primes_local, exp_local, cnt, 1, m, max_b, &beta);

        if (tau == (uint64_t)(beta << 1)) {
            seq_append((seq_t)m);
        }
    }
    return seq_buf[n - 1];
}
