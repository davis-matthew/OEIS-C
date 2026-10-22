/*
 * A342521  Heinz numbers of integer partitions with distinct first quotients.
 * https://oeis.org/A342521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest integer whose Heinz number partition has
 *            distinct first quotients
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates numbers in increasing order, tests each for the property and stores results. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd_u32.h"

/* ---------- Prime sieve and smallest prime factor ------------------------------------ */
static uint32_t *primes = NULL;          /* list of primes ↑ 1‑based index = array index+1     */
static uint32_t *prime_idx = NULL;       /* prime_idx[p] = position of p (1‑based), 0 if not prime */
static uint32_t *spf = NULL;             /* smallest prime factor of i                        */
static uint32_t sieve_limit = 0;
static uint32_t prime_cnt = 0;

/* Simple sieve of Eratosthenes that also fills spf and prime indices. */
static void init_sieve(void)
{
    if (sieve_limit) return;            /* already initialized */

    sieve_limit = 2000000;              /* large enough for the required tests            */
    primes = (uint32_t *)malloc(sieve_limit * sizeof(uint32_t));
    if (!primes) return;
    prime_idx = (uint32_t *)calloc(sieve_limit + 1, sizeof(uint32_t));
    spf = (uint32_t *)calloc(sieve_limit + 1, sizeof(uint32_t));
    if (!prime_idx || !spf) return;

    for (uint32_t i = 2; i <= sieve_limit; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            prime_idx[i] = ++prime_cnt;
            primes[prime_cnt - 1] = i;
            if ((uint64_t)i * i <= sieve_limit) {
                for (uint32_t j = i * i; j <= sieve_limit; j += i) {
                    if (!spf[j]) spf[j] = i;
                }
            }
        }
    }
}

/* Euclidean GCD for 32‑bit unsigned integers. */

/* ---------- Test whether a number satisfies the property ----------------------------- */
static bool is_valid_heinz(uint64_t m)
{
    if (m <= 1) return true;           /* 1 is accepted (empty partition) */

    /* collect prime indices with multiplicities */
    uint32_t idx_list[64];            /* should be enough for all relevant integers */
    size_t k = 0;
    uint64_t tmp = m;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        if (!p) p = tmp;              /* prime larger than sieve_limit (should not occur) */
        uint32_t idx = prime_idx[p];
        if (!idx) return false;       /* prime out of range */
        uint32_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        for (uint32_t e = 0; e < exp && k < 64; ++e) idx_list[k++] = idx;
    }

    /* sort indices descending */
    for (size_t i = 1; i < k; ++i) {
        uint32_t v = idx_list[i];
        size_t j = i;
        while (j > 0 && idx_list[j - 1] < v) {
            idx_list[j] = idx_list[j - 1];
            --j;
        }
        idx_list[j] = v;
    }

    /* check distinctness of reduced first quotients */
    if (k <= 1) return true;
    struct fraction { uint32_t num; uint32_t den; } seen[64];
    size_t seen_cnt = 0;
    for (size_t i = 0; i + 1 < k; ++i) {
        uint32_t den = idx_list[i];
        uint32_t num = idx_list[i + 1];
        uint32_t g = gcd_u32(num, den);
        num /= g; den /= g;
        for (size_t j = 0; j < seen_cnt; ++j) {
            if (seen[j].num == num && seen[j].den == den) return false;
        }
        seen[seen_cnt++] = (struct fraction){num, den};
    }
    return true;
}

/* ---------- Sequence storage ---------------------------------------------------------- */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0, seq_cap = 0;
static uint64_t current_val = 0;
static bool seq_initialized = false;

/* grow array if needed */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    while (seq_cap < needed) seq_cap = seq_cap ? seq_cap * 2 : 16;
    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
}

/* ---------- Main function ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!seq_initialized) {
        init_sieve();
        seq_cap = 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        current_val = 0;
        seq_initialized = true;
    }

    /* generate until we have n terms */
    while (seq_len < (size_t)n) {
        ++current_val;
        if (is_valid_heinz(current_val)) {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)current_val;
        }
    }
    return seq_arr[n - 1];
}
