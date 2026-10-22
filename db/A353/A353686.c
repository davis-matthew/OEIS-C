/*
 * A353686  Numbers k for which phi(sigma(k)) < phi(k).
 * https://oeis.org/A353686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k‑th integer k satisfying φ(σ(k)) < φ(k)
 * Range:     u8 n <= 45, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning integers and testing the condition with
   precomputed φ and σ values (sieve + trial division). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const idx_t LIMIT = 2000000;          /* enough for n up to ~10⁴ */
static seq_t *phi = NULL;
static seq_t *sigma = NULL;
static uint32_t *spf = NULL;                /* smallest prime factor */
static uint32_t *prime_arr = NULL;          /* list of primes up to LIMIT     */
static size_t prime_cnt = 0;

/* result cache */
static seq_t *res_cache = NULL;
static size_t res_len = 0;
static size_t res_cap = 0;
static idx_t next_k = 2;

static bool initialized = false;

/* phi of an arbitrary value using the prime list */
static seq_t phi_of_value(seq_t x)
{
    seq_t result = x;
    seq_t rem = x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_arr[i];
        seq_t pp = (seq_t)p;
        if (pp * pp > rem) break;
        if (rem % p == 0) {
            while (rem % p == 0) rem /= p;
            result -= result / pp;
        }
    }
    if (rem > 1) result -= result / rem;
    return result;
}

/* initialise sieve, phi, sigma and prime list */
static void init(void)
{
    if (initialized) return;

    /* allocate arrays */
    phi   = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    sigma = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    spf   = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));

    /* sufficient space for primes (≈ LIMIT / log LIMIT) */
    size_t prime_space = LIMIT / 10 + 10;
    prime_arr = (uint32_t *)malloc(prime_space * sizeof(uint32_t));

    /* linear sieve for spf and prime list */
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
    size_t pc = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (pc < prime_space) prime_arr[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc && prime_arr[j] <= spf[i] &&
               i <= LIMIT / prime_arr[j]; ++j) {
            spf[i * prime_arr[j]] = prime_arr[j];
        }
    }
    prime_cnt = pc;

    /* phi and sigma */
    phi[1] = (seq_t)1;
    sigma[1] = (seq_t)1;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        uint32_t p = spf[i];
        idx_t tmp = i;
        size_t exp = 0;
        while (tmp % p == 0) { tmp /= p; ++exp; }

        /* compute p^exp */
        seq_t p_pow = 1;
        for (size_t e = 0; e < exp; ++e) p_pow *= (seq_t)p;

        /* sigma part: 1 + p + ... + p^exp */
        seq_t sigma_part = 1;
        seq_t cur_pow = 1;
        for (size_t e = 0; e < exp; ++e) {
            cur_pow *= (seq_t)p;
            sigma_part += cur_pow;
        }

        /* phi part: p^(exp-1)*(p-1) = (p_pow/p)*(p-1) */
        seq_t phi_part = (p_pow / p) * (p - 1);

        phi[i] = phi_part * phi[tmp];
        sigma[i] = sigma_part * sigma[tmp];
    }

    initialized = true;
}

/* append value to the result cache */
static void append_res(seq_t v)
{
    if (res_len + 1 > res_cap) {
        size_t new_cap = res_cap ? res_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(res_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        res_cache = tmp;
        res_cap = new_cap;
    }
    res_cache[res_len++] = v;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init();

    while (res_len < n) {
        if (next_k > LIMIT) {
            /* insufficient precomputation – unlikely for n ≤ 10⁴ */
            exit(1);
        }
        seq_t cur_phi = phi[next_k];
        seq_t cur_sigma = sigma[next_k];
        seq_t phi_sigma = phi_of_value(cur_sigma);

        if (phi_sigma < cur_phi) {
            append_res((seq_t)next_k);
        }
        ++next_k;
    }
    return res_cache[n - 1];
}
