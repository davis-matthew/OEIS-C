/*
 * A246362  Numbers n such that if 2n-1 = Product_{k >= 1} (p_k)^(c_k), then n
 *          < Product_{k >= 1} (p_{k-1})^(c_k), where p_k indicates the k-th
 *          prime, A000040(k).
 * https://oeis.org/A246362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_check_overflow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A246362 - numbers n such that if 2n-1 = ∏p_k^c_k then
   n < ∏p_{k-1}^c_k.  Computed by linear search with a prime sieve. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mul_check_overflow.h"

/* ---------- Prime sieve for numbers up to 1 000 000 ---------- */
#define MAX_PRIME 1000000

static uint32_t *g_primes = NULL;     /* 1‑based array of primes */
static size_t g_prime_cnt = 0;       /* number of primes in g_primes */
static int *g_prime_idx = NULL;      /* index of a prime (1‑based), 0 if not prime */
static bool g_primes_initialized = false;

static void primes_init(void)
{
    if (g_primes_initialized) return;
    g_primes_initialized = true;

    /* Allocate temporary sieve */
    static uint8_t sieve[MAX_PRIME + 1];
    memset(sieve, 1, sizeof(sieve));
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes */
    g_prime_cnt = 0;
    for (size_t i = 2; i <= MAX_PRIME; ++i)
        if (sieve[i]) ++g_prime_cnt;

    /* Allocate arrays */
    g_primes   = (uint32_t*)malloc((g_prime_cnt + 1) * sizeof(uint32_t)); /* 1‑based */
    g_prime_idx = (int*)malloc((MAX_PRIME + 1) * sizeof(int));
    memset(g_prime_idx, 0, (MAX_PRIME + 1) * sizeof(int));

    /* Fill arrays */
    size_t idx = 1;
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            g_primes[idx] = (uint32_t)i;
            g_prime_idx[i] = (int)idx;
            ++idx;
        }
    }
}

/* Test whether a candidate satisfies the inequality. */
static bool ok_candidate(uint64_t cand)
{
    uint64_t x = 2 * cand - 1;          /* odd */
    uint64_t tmp = x;
    seq_t prod_low = 1;

    /* Factor using primes up to sqrt(x) */
    for (size_t pi = 1; pi <= g_prime_cnt && (uint64_t)g_primes[pi] * (uint64_t)g_primes[pi] <= tmp; ++pi) {
        uint64_t p = g_primes[pi];
        if (tmp % p == 0) {
            unsigned int exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }

            int idx = g_prime_idx[p];
            if (idx <= 1) return false;          /* should not occur (p=2) */

            uint64_t lower_p = g_primes[idx - 1]; /* p_{k-1} */
            seq_t pow_low = 1;
            for (unsigned int i = 0; i < exp; ++i) {
                pow_low = mul_check_overflow(pow_low, (seq_t)lower_p, (seq_t)cand);
                if (pow_low > (seq_t)cand) break; /* no need to compute further */
            }

            prod_low = mul_check_overflow(prod_low, pow_low, (seq_t)cand);
            if (prod_low > (seq_t)cand) return (cand < prod_low); /* false */
        }
    }

    /* Remaining prime factor > sqrt(x) */
    if (tmp > 1) {
        uint64_t p = tmp;
        int idx = g_prime_idx[p];
        if (idx <= 1) return false;
        uint64_t lower_p = g_primes[idx - 1];
        prod_low = mul_check_overflow(prod_low, (seq_t)lower_p, (seq_t)cand);
    }

    return (cand < prod_low);
}

/* ---------- Sequence cache ---------- */
static seq_t *g_seq = NULL;     /* dynamic array of computed terms */
static size_t g_seq_cap = 0;    /* capacity of g_seq */
static size_t g_seq_len = 0;    /* number of terms stored */

/* Ensure space for at least n+1 elements (1‑based). */
static void ensure_series_capacity(size_t n)
{
    if (g_seq_cap > n) return;
    size_t new_cap = (g_seq_cap == 0) ? 16 : g_seq_cap * 2;
    if (new_cap < n + 1) new_cap = n + 1;
    seq_t *new_arr = (seq_t*)realloc(g_seq, new_cap * sizeof(seq_t));
    g_seq = new_arr;
    g_seq_cap = new_cap;
}

/* Generate terms up to n (1‑based). */
static void generate_up_to(idx_t n)
{
    if (g_seq_len >= (size_t)n) return;
    if (!g_primes_initialized) primes_init();

    uint64_t candidate = (g_seq_len == 0) ? 1 : (uint64_t)g_seq[g_seq_len - 1] + 1;
    while (g_seq_len < (size_t)n) {
        if (ok_candidate(candidate)) {
            ensure_series_capacity(g_seq_len + 1);
            g_seq[g_seq_len] = (seq_t)candidate;
            ++g_seq_len;
        }
        ++candidate;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, but safe */
    generate_up_to(n);
    return g_seq[n - 1];
}
