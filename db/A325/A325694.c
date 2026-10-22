/*
 * A325694  Numbers with one fewer divisors than the sum of their prime
 *          indices.
 * https://oeis.org/A325694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = all n such that d(n) = sum of prime indices of n – 1. The
 *            function a(idx_t n) returns the nth such number (n ≥ 1)
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*  Pre‑compute primes up to LIMIT and store 1‑based indices.  */
#define LIMIT          2000000      /* enough for n up to ~10 000 */

static uint32_t *primes = NULL;          /* array of primes */
static size_t    prime_count = 0;         /* number of primes computed */
static uint32_t *prime_index = NULL;      /* prime_index[p] = its 1‑based index; 0 if composite */

/* One‑time sieve of Eratosthenes.  Safe to call from a() any time. */
static void
init_primes(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* Allocate storage for primes and their indices. */
    primes       = (uint32_t*)malloc((LIMIT + 1000) * sizeof(uint32_t));
    prime_index  = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!primes || !prime_index) {
        exit(EXIT_FAILURE);              /* allocation failure */
    }

    /* Sieve with a bit array. */
    uint8_t *sieve = (uint8_t*)calloc(LIMIT + 1, 1);
    if (!sieve) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {                       /* i is prime */
            primes[idx]       = i;
            prime_index[i]    = (uint32_t)(idx + 1);  /* 1‑based */
            ++idx;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) {
                    sieve[j] = 1;
                }
            }
        }
    }
    prime_count = idx;
    free(sieve);
}

/* Compute τ(n) (number of divisors) and sum of prime indices of n.
   Both results are returned in seq_t (the build‑dependent type). */
static void
tau_and_sum_indices(uint64_t n, seq_t *tau, seq_t *sumidx)
{
    uint64_t r = n;
    seq_t t = 1;
    seq_t s = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > r) break;
        if (r % p == 0) {
            uint64_t e = 0;
            while (r % p == 0) {
                r /= p;
                ++e;
            }
            t *= (seq_t)(e + 1);
            s += (seq_t)e * (seq_t)prime_index[p];
        }
    }
    if (r > 1) {                     /* remaining prime factor */
        t *= (seq_t)2;
        s += (seq_t)prime_index[(uint32_t)r];
    }
    *tau    = t;
    *sumidx = s;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n = 0, return 0 */

    init_primes();

    idx_t found = 0;
    seq_t result = 0;

    for (uint64_t num = 2;; ++num) {
        seq_t tau, sumidx;
        tau_and_sum_indices(num, &tau, &sumidx);

        if (tau == sumidx - 1) {    /* condition satisfied */
            ++found;
            if (found == n) {
                result = (seq_t)num;
                break;
            }
        }
    }
    return result;
}
