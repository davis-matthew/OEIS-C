/*
 * A256651  Numbers k for which both the number of evil numbers <= k and
 *          relatively prime to k, and the number of odious numbers <= k and
 *          relatively prime to k, are primes.
 * https://oeis.org/A256651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation: a(n) = the nth integer k such that
 *   (1) the count of evil numbers ≤ k that are coprime with k is prime,
 *   (2) the count of odious numbers ≤ k that are coprime with k is prime.
 *
 * The function builds a small sieve up to MAX_K and queries numbers
 * sequentially, storing the found k's in a dynamic array of 64‑bit
 * integers (no construction of seq_t objects required).  It is
 * memoised so repeated calls work in O(1) for already computed terms.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"

#define MAX_K 200000   /* maximum value pre‑computed; should cover n≤10000 */

static int8_t *mu_arr    = NULL;   /* Möbius function for 1..MAX_K */
static uint32_t *spf_arr = NULL;   /* smallest prime factor for 1..MAX_K */
static unsigned char *evil_flag = NULL; /* 1 if number is evil, else 0 */
static uint32_t *offset_arr = NULL;     /* starting index of each d in evil_prefix */
static uint32_t *evil_prefix = NULL;   /* packed prefix sums per divisor */
static size_t prefix_total = 0;        /* total length of evil_prefix */

static bool init_done = false;

/* ---------------------------------------------------------------------- */
/*  Utility: parity of number of ones in binary representation           */
/* ---------------------------------------------------------------------- */
static inline bool is_evil(uint32_t x)
{
    return (__builtin_popcountll((unsigned long long)x) & 1) == 0;
}

/* ---------------------------------------------------------------------- */
/*  One‑time initialisation: sieve, μ, evil flag, prefix tables          */
/* ---------------------------------------------------------------------- */
static void init_constants(void)
{
    if (init_done) return;
    init_done = true;

    /* allocate basic arrays */
    mu_arr      = (int8_t   *)calloc(MAX_K + 1, sizeof(int8_t));
    spf_arr     = (uint32_t *)calloc(MAX_K + 1, sizeof(uint32_t));
    evil_flag   = (unsigned char *)calloc(MAX_K + 1, sizeof(unsigned char));
    offset_arr  = (uint32_t *)calloc(MAX_K + 1, sizeof(uint32_t));

    /* linear sieve for spf and μ */
    uint32_t *pr = (uint32_t *)malloc((MAX_K / 10) * sizeof(uint32_t));
    size_t pr_cnt = 0;
    mu_arr[1] = 1;
    for (uint32_t i = 2; i <= MAX_K; ++i) {
        if (!spf_arr[i]) {
            spf_arr[i] = i;
            pr[pr_cnt++] = i;
            mu_arr[i] = -1;
        }
        for (size_t j = 0; j < pr_cnt && pr[j] <= spf_arr[i] &&
             (uint64_t)i * pr[j] <= (uint64_t)MAX_K; ++j) {
            uint32_t v = i * pr[j];
            spf_arr[v] = pr[j];
            if (i % pr[j] == 0) { mu_arr[v] = 0; break; }
            mu_arr[v] = -mu_arr[i];
        }
    }
    free(pr);

    /* build evil_flag */
    for (uint32_t i = 1; i <= MAX_K; ++i)
        evil_flag[i] = is_evil(i) ? 1 : 0;

    /* compute total size for evil_prefix (sum_{d≤N} N/d) */
    prefix_total = 0;
    for (uint32_t d = 1; d <= MAX_K; ++d)
        prefix_total += MAX_K / d;
    evil_prefix = (uint32_t *)malloc(prefix_total * sizeof(uint32_t));

    /* fill prefix sums per divisor */
    uint32_t pos = 0;
    for (uint32_t d = 1; d <= MAX_K; ++d) {
        offset_arr[d] = pos;
        uint32_t running = 0;
        uint32_t cnt = MAX_K / d;
        for (uint32_t i = 1; i <= cnt; ++i) {
            uint32_t m = d * i;
            if (evil_flag[m]) ++running;
            evil_prefix[pos++] = running;
        }
    }
}

/* ---------------------------------------------------------------------- */
/*  Dynamic array for found k values (64‑bit integers)                     */
/* ---------------------------------------------------------------------- */
static uint64_t *seq_vals64 = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static uint32_t next_k = 1;   /* next k to evaluate */

/* ---------------------------------------------------------------------- */
/*  Main function                                                        */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise static data at first call */
    static bool first_called = false;
    if (!first_called) {
        init_constants();
        first_called = true;
    }

    /* compute until we have at least n terms */
    while (seq_len < (size_t)n) {
        uint32_t k = next_k++;

        /* factorisation of k using spf_arr */
        uint32_t tmp = k;
        uint32_t prime_f[32];
        int  exp_f[32];
        size_t pcnt = 0;
        while (tmp > 1) {
            uint32_t p = spf_arr[tmp];
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            prime_f[pcnt] = p;
            exp_f[pcnt] = e;
            ++pcnt;
        }

        /* enumerate all divisors of k */
        uint32_t divs[256];
        size_t dcnt = 1;
        divs[0] = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            uint32_t p = prime_f[i];
            int  e = exp_f[i];
            uint32_t mult = 1;
            size_t curcnt = dcnt;
            for (int ee = 1; ee <= e; ++ee) {
                mult *= p;
                for (size_t j = 0; j < curcnt; ++j)
                    divs[dcnt++] = divs[j] * mult;
            }
        }

        /* Möbius inversion for evil count */
        int64_t tot_evil = 0;
        for (size_t i = 0; i < dcnt; ++i) {
            uint32_t d = divs[i];
            int8_t mu = mu_arr[d];
            if (mu == 0) continue;
            uint32_t idx = k / d;          /* 1‑based index in multiples of d */
            uint32_t ge = evil_prefix[offset_arr[d] + idx - 1];
            tot_evil += (int64_t)mu * (int64_t)ge;
        }
        seq_t cev = (seq_t)tot_evil;

        /* φ(k) from its prime factors */
        seq_t phi = k;
        for (size_t i = 0; i < pcnt; ++i)
            phi -= phi / prime_f[i];
        seq_t cod = phi - cev;    /* odious count = φ(k) – evil count */

        /* test primality of both counts */
        if (isPrimeGeneric(cev) && isPrimeGeneric(cod)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap << 1 : 64;
                seq_vals64 = (uint64_t *)realloc(seq_vals64,
                                                 seq_cap * sizeof(uint64_t));
            }
            seq_vals64[seq_len++] = k;
        }
    }
    return (seq_t)seq_vals64[n - 1];
}
