/*
 * A246676  Permutation of natural numbers: a(n) = A242378(A007814(n),
 *          (1+A000265(n))) - 1.
 * https://oeis.org/A246676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A242378(A007814(n), (1+A000265(n))) - 1 (implementation
 *            based on the binary‑trailing‑zeros description)
 * Range:     u8 n <= 55, u16 n <= 495, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum value for precomputed primes (satisfies the required index range). */
#define PRIME_LIMIT 20000

/* Static data for primes */
static bool primes_inited = false;
static uint32_t primes[PRIME_LIMIT];     /* primes[0] = 2, ... */
static size_t  prime_cnt = 0;
static uint16_t prime_index[PRIME_LIMIT + 1];  /* 1‑based indices of primes */

/* Initializer: sieve of Eratosthenes up to PRIME_LIMIT */
static void init_primes(void)
{
    if (primes_inited)
        return;

    /* A small boolean array for the sieve */
    uint8_t is_prime[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= PRIME_LIMIT; q += p)
                is_prime[q] = 0;
        }
    }

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt] = (uint32_t)i;
            prime_index[i] = (uint16_t)(prime_cnt + 1);  /* 1‑based */
            ++prime_cnt;
        }
    }

    primes_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure primes are ready */
    init_primes();

    /* Step 1: find k = number of trailing zeros of n and reduce n */
    idx_t t = n;
    idx_t k = 0;
    while ((t & (idx_t)1) == 0) {
        ++k;
        t >>= 1;
    }

    /* Step 2: odd part and m = odd + 1 (even) */
    seq_t odd = (seq_t)t;
    seq_t m   = odd + 1;   /* m <= n+1, fits in uint64_t for our limits */

    /* Step 3: factor m and compute the shifted product */
    uint64_t mm = (uint64_t)m;    /* work with native integers for factoring */
    seq_t product = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= mm; ++i) {
        uint32_t p = primes[i];
        if (mm % p == 0) {
            int exp = 0;
            while (mm % p == 0) {
                ++exp;
                mm /= p;
            }
            uint16_t idx_p   = prime_index[p];          /* 1‑based index of p */
            uint16_t new_idx = idx_p + (uint16_t)k;     /* shift by k */
            seq_t new_p      = (seq_t)primes[new_idx - 1]; /*0‑based array access*/

            while (exp--) {
                product *= new_p;
            }
        }
    }

    if (mm > 1) {  /* Remaining prime factor (mm is prime) */
        uint32_t p = (uint32_t)mm;
        uint16_t idx_p   = prime_index[p];
        uint16_t new_idx = idx_p + (uint16_t)k;
        product *= (seq_t)primes[new_idx - 1];
    }

    /* Step 4: subtract 1 */
    return product - (seq_t)1;
}
