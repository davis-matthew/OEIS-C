/*
 * A374122  Numbers k for which A328845(k) is a multiple of 3, where A328845
 *          is the first Fibonacci-based variant of the arithmetic derivative.
 * https://oeis.org/A374122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k for which A328845(k) is a multiple of 3. This is
 *            computed by checking whether the Fibonacci‑based arithmetic
 *            derivative d(k) ≡ 0 (mod 3)
 * Range:     u8 n <= 82, u16 n <= 9903, u32 n <= 9903, u64 n <= 9903,
 *            u128 n <= 9903, big any n
 * Helpers:   fib_mod3_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/fib_mod3_prime.h"

/* Smallest prime factor array (SPF) up to a fixed limit.  */
static uint32_t *spf = NULL;
static size_t spf_limit = 0;
static const size_t SPF_INIT_LIMIT = 2000000u; /* enough for first 10 000 terms */

static void init_spf(void)
{
    if (spf) return;              /* already initialised */
    size_t limit = SPF_INIT_LIMIT;
    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    /* linear sieve */
    uint32_t *primes = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    size_t pc = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            primes[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc && primes[j] <= spf[i] &&
                     i * (size_t)primes[j] <= limit; ++j) {
            spf[i * (size_t)primes[j]] = primes[j];
        }
    }
    free(primes);
    spf_limit = limit;
}

/* Test if integer n has derivative divisible by 3.  */
static bool good_candidate(idx_t n)
{
    if (n <= 1) return true;                 /* 0 and 1 are always good */

    int d_mod3 = 0;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        int fibm = fib_mod3_prime(p);
        if (fibm == 0) continue;           /* contribution is 0 mod 3 */
        int term = (e % 3) * fibm;          /* e·F(p) mod 3 */
        term %= 3;
        int n_div_p_mod3 = (int)((n / p) % 3);
        term = (term * n_div_p_mod3) % 3;
        d_mod3 += term;
        d_mod3 %= 3;
    }
    return d_mod3 == 0;
}

/* Cache for the sequence.  */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0, seq_len = 0;
static idx_t next_candidate = 2;

/* Ensure the cache has at least n elements.  */
static void ensure_terms(idx_t n)
{
    if (!seq_cache) {
        seq_cap = 8;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        if (seq_len + 2 > seq_cap) {
            seq_cap = 16;
            seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
        }
        seq_cache[seq_len++] = (seq_t)0;   /* a(1) = 0 */
        seq_cache[seq_len++] = (seq_t)1;   /* a(2) = 1 */
    }

    init_spf();                          /* initialise SPF once */

    while (seq_len < n) {
        if (good_candidate(next_candidate)) {
            if (seq_len >= seq_cap) {
                seq_cap <<= 1;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe return */
    ensure_terms(n);
    return seq_cache[n - 1];
}
