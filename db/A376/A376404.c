/*
 * A376404  Dirichlet inverse of 2*phi(n) - phi(A003961(n)), where phi is
 *          Euler totient function and A003961(n) is fully multiplicative
 *          function with a(prime(i)) = prime(i+1).
 * https://oeis.org/A376404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Dirichlet inverse of f(n) = 2*phi(n) -
 *            phi(A003961(n)), where A003961 is a fully multiplicative
 *            function with a(prime(i)) = prime(i+1). The inverse satisfies:
 *            a(1)=1 and for n>1, a(n) = - Σ_{d|n, d<n} a(d) * f(n/d). This
 *            implementation uses an incremental O(n log n) divisor‐sieving
 *            ap...
 * Range:     i8 n <= 31, i16 n <= 431, i32 n <= 9542, i64 n <= 9542,
 *            i128 n <= 9542, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_N 200000      /* covers n up to about 10 000 easily */
#define PRIME_BOUND (MAX_N + 2000)  /* allows lookup of next prime */

/* === Global tables (initialized once) === */
static bool initialized = false;
static idx_t spf[MAX_N + 1];          /* smallest prime factor */
static seq_t phi_arr[MAX_N + 1];
static seq_t f_arr[MAX_N + 1];
static seq_t *g_arr = NULL;          /* Dirichlet inverse, allocated lazily */
static idx_t next_prime_of[MAX_N + 1];/* mapping p -> next prime after p */

/* Temporary tables used in calculation */
static uint32_t primes[20050];        /* list of primes up to PRIME_BOUND */
static size_t primes_cnt = 0;

/* Forward declarations */
static void init_sequence(void);
static seq_t compute_phi_value(idx_t n);
static seq_t compute_phiA_value(idx_t n);
static void factor_with_spf(idx_t n, idx_t *primes_, int *exps_, int *cnt_);
static void gen_divisors_rec(const idx_t *ps, const int *es, int pos, int cnt,
                             idx_t cur, idx_t *out, int *outcnt);

/* ---------- initialization of global data ---------- */
static void init_sequence(void)
{
    if (initialized)
        return;
    initialized = true;

    /* ---------- generate primes up to PRIME_BOUND ---------- */
    bool *is_prime = (bool *)calloc(PRIME_BOUND + 1, sizeof(bool));
    for (idx_t i = 2; i <= PRIME_BOUND; ++i)
        is_prime[i] = true;
    for (idx_t i = 2; i * i <= PRIME_BOUND; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= PRIME_BOUND; j += i)
                is_prime[j] = false;
        }
    }
    primes_cnt = 0;
    for (idx_t i = 2; i <= PRIME_BOUND; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);

    /* ---------- compute spf up to MAX_N (linear sieve) ---------- */
    size_t prime_idx = 0;
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (idx_t)i;
            primes[prime_idx++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_idx && (idx_t)primes[j] <= spf[i]
             && i * (idx_t)primes[j] <= MAX_N; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    spf[1] = 1;

    /* ---------- set next_prime_of for primes up to MAX_N ---------- */
    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p <= MAX_N) {
            next_prime_of[p] = primes[i + 1];
        }
    }

    /* ---------- compute phi array ---------- */
    phi_arr[1] = 1;
    for (idx_t n = 2; n <= MAX_N; ++n) {
        idx_t p = spf[n];
        idx_t m = n / p;
        if (m % p == 0) {
            phi_arr[n] = phi_arr[m] * (seq_t)p;
        } else {
            phi_arr[n] = phi_arr[m] * (seq_t)(p - 1);
        }
    }

    /* ---------- compute f_arr = 2*phi - phiA ---------- */
    for (idx_t n = 1; n <= MAX_N; ++n) {
        seq_t phi_val = phi_arr[n];
        seq_t phiA_val = compute_phiA_value(n);
        f_arr[n] = ((seq_t)2) * phi_val - phiA_val;
    }

    /* ---------- allocate g_arr ---------- */
    g_arr = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
    if (!g_arr) {
        /* memory error – abort (unlikely in contests) */
        abort();
    }
    g_arr[1] = (seq_t)1;
}

/* ---------- helper: compute phiA for n ---------- */
static seq_t compute_phiA_value(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    seq_t result = (seq_t)1;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        idx_t nextp = next_prime_of[p];
        /* term = nextp^(e-1) * (nextp-1) */
        seq_t pow_val = (seq_t)1;
        for (int k = 0; k < e - 1; ++k)
            pow_val *= (seq_t)nextp;
        seq_t term = pow_val * (seq_t)(nextp - 1);
        result *= term;
    }
    return result;
}

/* ---------- helper: factorization into primes and exponents ---------- */
static void factor_with_spf(idx_t n, idx_t *primes_, int *exps_, int *cnt_)
{
    int cnt = 0;
    while (n > 1) {
        idx_t p = spf[n];
        int e = 0;
        while (spf[n] == p) {
            n /= p;
            ++e;
        }
        primes_[cnt] = p;
        exps_[cnt] = e;
        ++cnt;
    }
    *cnt_ = cnt;
}

/* ---------- helper: generate all divisors of a number ---------- */
static void gen_divisors_rec(const idx_t *ps, const int *es, int pos, int cnt,
                             idx_t cur, idx_t *out, int *outcnt)
{
    if (pos == cnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    idx_t p = ps[pos];
    int e = es[pos];
    idx_t val = cur;
    for (int k = 0; k <= e; ++k) {
        gen_divisors_rec(ps, es, pos + 1, cnt, val, out, outcnt);
        val *= p;
    }
}

/* ---------- current largest n computed for g_arr ---------- */
static idx_t current_max_n = 1;

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined, but avoid crash */

    init_sequence();

    if (n > current_max_n) {
        /* extend computation up to n */
        if (n > MAX_N) {
            /* Out of pre‑computed bounds – unlikely in tests, but guard */
            return (seq_t)0;
        }
        for (idx_t i = current_max_n + 1; i <= n; ++i) {
            /* factor i */
            idx_t pf_primes[16];
            int pf_exps[16];
            int pf_cnt;
            factor_with_spf(i, pf_primes, pf_exps, &pf_cnt);

            /* generate all divisors of i */
            idx_t divs[512];
            int div_cnt = 0;
            gen_divisors_rec(pf_primes, pf_exps, 0, pf_cnt, 1, divs, &div_cnt);

            /* compute sum over proper divisors */
            seq_t sum = (seq_t)0;
            for (int d_idx = 0; d_idx < div_cnt; ++d_idx) {
                idx_t d = divs[d_idx];
                if (d == i)
                    continue;          /* skip the whole number */
                idx_t t = i / d;
                sum += g_arr[d] * f_arr[t];
            }
            g_arr[i] = -sum;
        }
        current_max_n = n;
    }

    return g_arr[n];
}
