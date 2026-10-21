/*
 * A275476  Least k such that n divides d(k!) where d = A000005 (k > 0).
 * https://oeis.org/A275476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that n divides τ(k!) where τ is the number
 *            of divisors (A000005)
 * Range:     u8 n <= 136, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the value by iterating k and maintaining the prime exponents
 * in k!. The τ(k!) is evaluated modulo n each step, so large intermediate values are
 * never produced. The routine works for any integer type seq_t that satisfies the
 * arithmetic operations in the description. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_K   200000  /* upper bound for the search; ample for n up to 10⁴ */

static int initialized = 0;
static uint32_t *spf      = NULL;   /* smallest prime factor for each number <= MAX_K */
static uint32_t *primes   = NULL;   /* list of primes <= MAX_K */
static int       *prime_id = NULL;  /* prime_id[p] gives index of p in primes array, -1 otherwise */
static size_t    prime_cnt = 0;
static uint32_t *exp_cnt  = NULL;   /* exponent of each prime in current k! */

static void init_sieve(void)
{
    spf = (uint32_t*)calloc(MAX_K + 1, sizeof(uint32_t));
    prime_id = (int*)calloc(MAX_K + 1, sizeof(int));
    for (uint32_t i = 0; i <= MAX_K; ++i) spf[i] = i;
    for (uint32_t i = 2; i * i <= MAX_K; ++i)
        if (spf[i] == i)
            for (uint32_t j = i * i; j <= MAX_K; j += i)
                if (spf[j] == j) spf[j] = i;

    primes = (uint32_t*)malloc((MAX_K / 10) * sizeof(uint32_t)); /* rough estimate */
    prime_cnt = 0;
    for (uint32_t i = 2; i <= MAX_K; ++i)
        if (spf[i] == i) {
            primes[prime_cnt++] = i;
            prime_id[i] = (int)prime_cnt - 1;
        }
    exp_cnt = (uint32_t*)calloc(prime_cnt, sizeof(uint32_t));
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sieve();

    /* Special case: a(1) = 1 */
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t N = (uint64_t)n;          /* modulus for divisibility test */
    memset(exp_cnt, 0, prime_cnt * sizeof(uint32_t));

    for (uint32_t k = 1; ; ++k)
    {
        /* Update prime exponents for the new factor k */
        uint32_t x = k;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            exp_cnt[prime_id[p]] += cnt;
        }

        /* Compute τ(k!) mod N */
        uint64_t tau_mod = 1 % N;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t e = exp_cnt[i];
            if (!e) continue;
            uint64_t factor = e + 1;
            tau_mod = (unsigned __int128)tau_mod * factor % N;
        }

        if (tau_mod == 0) return (seq_t)k;
    }
}
