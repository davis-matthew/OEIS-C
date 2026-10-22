/*
 * A338331  Numbers whose set of distinct prime indices (A304038) is pairwise
 *          coprime, where a singleton is always considered coprime.
 * https://oeis.org/A338331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth number whose distinct prime indices are pairwise
 *            coprime (OEIS A338331). The sequence is computed by testing
 *            integers in increasing order and storing the first n qualifying
 *            numbers. The algorithm uses a sieve of Eratosthenes up to a
 *            fixed limit (10 000 000) which is sufficient for the...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---- utility: gcd for 32‑bit integers ----------------------------------- */
static uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b != 0) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---- sieve tables ------------------------------------------------------- */
#define SIEVE_LIMIT 10000000U        /* primes up to 10 000 000 */

/* prime index (1‑based) for primes <= SIEVE_LIMIT; 0 for composite numbers */
static uint32_t *prime_idx = NULL;
/* list of primes in increasing order */
static uint32_t *primes = NULL;
/* number of primes found */
static size_t primes_cnt = 0;

/* initialise sieve and tables once */
static void init_sieve(void)
{
    if (prime_idx != NULL) return;   /* already done */

    /* allocate arrays */
    prime_idx = (uint32_t*)calloc((size_t)SIEVE_LIMIT + 1, sizeof(uint32_t));
    /* estimate capacity: about SIEVE_LIMIT / ln(SIEVE_LIMIT) */
    size_t est_cap = (size_t)(SIEVE_LIMIT / 10) + 1024;
    primes = (uint32_t*)malloc(est_cap * sizeof(uint32_t));

    uint8_t *is_comp = (uint8_t*)calloc((size_t)SIEVE_LIMIT + 1, sizeof(uint8_t));
    primes_cnt = 0;

    for (uint32_t i = 2; i <= (uint32_t)SIEVE_LIMIT; ++i) {
        if (is_comp[i]) continue;
        /* i is prime */
        if (primes_cnt == est_cap) {
            est_cap <<= 1;
            primes = (uint32_t*)realloc(primes, est_cap * sizeof(uint32_t));
        }
        primes[primes_cnt] = i;
        primes_cnt++;
        prime_idx[i] = (uint32_t)primes_cnt;   /* 1‑based index */

        if ((uint64_t)i * (uint64_t)i <= (uint64_t)SIEVE_LIMIT) {
            for (uint32_t j = i * i; j <= (uint32_t)SIEVE_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }
    free(is_comp);
}

/* ---- test whether a number satisfies the pairwise coprime prime‑index condition */
static bool good_candidate(uint64_t n)
{
    if (n == 1) return true;

    uint32_t indices[64];
    size_t idx_cnt = 0;
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t idx = prime_idx[p];
            while (tmp % p == 0) tmp /= p;
            for (size_t j = 0; j < idx_cnt; ++j) {
                if (gcd32(idx, indices[j]) != 1) return false;
            }
            indices[idx_cnt++] = idx;
        }
    }
    if (tmp > 1) {   /* remaining prime factor > sqrt(original) */
        uint32_t p = (uint32_t)tmp;
        if (p > SIEVE_LIMIT) return false;   /* beyond sieve – should not happen */
        uint32_t idx = prime_idx[p];
        for (size_t j = 0; j < idx_cnt; ++j) {
            if (gcd32(idx, indices[j]) != 1) return false;
        }
        indices[idx_cnt++] = idx;
    }
    return true;
}

/* ---- sequence storage ----------------------------------------------------- */
static seq_t *seq_vals = NULL;   /* array of sequence values */
static size_t seq_len = 0;       /* current number of stored values */
static size_t seq_cap = 0;       /* allocated capacity */
static uint64_t next_candidate = 1;   /* next integer to test */

/* ensure the first n terms are computed */
static void ensure_sequence(idx_t n)
{
    init_sieve();

    if (seq_vals == NULL) {
        seq_cap = 128;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    while (seq_len < (size_t)n) {
        while (true) {
            if (good_candidate((uint64_t)next_candidate)) {
                if (seq_len >= seq_cap) {
                    seq_cap *= 2;
                    seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
                }
                seq_vals[seq_len++] = (seq_t)next_candidate;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
}

/* --- public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1; return 0 as a safe default */
    ensure_sequence(n);
    return seq_vals[(size_t)n - 1];
}
