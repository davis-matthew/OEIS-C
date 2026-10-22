/*
 * A306824  Integer k such that digsum(k) = digsum (k/p(1)) = digsum (k/p(2))
 *          = ... for all prime factors p(i) of k, where digsum(k) =
 *          A007953(k) is the digital sum of k.
 * https://oeis.org/A306824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A306824 – numbers k such that digsum(k) = digsum(k/p) for every
 *            prime factor p of k
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIME 1000000  /* enough to factor numbers up to ~1e12 */

/* Static data for primes */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* ------------------------------------------------------------------*/
/* Helper: compute digit sum of a 64‑bit integer                       */
static seq_t digit_sum_uint64(uint64_t n)
{
    seq_t sum = 0;
    while (n) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

/* Helper: generate all primes up to MAX_PRIME using a simple sieve   */
static void init_primes(void)
{
    if (primes != NULL)
        return;                       /* already initialized */

    /* Allocate sieve */
    bool *sieve = (bool *)malloc((MAX_PRIME + 1) * sizeof(bool));
    memset(sieve, 1, (MAX_PRIME + 1) * sizeof(bool));
    sieve[0] = sieve[1] = false;

    /* Sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (sieve[i])
            ++primes_cnt;

    /* Store primes */
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (sieve[i])
            primes[idx++] = i;

    free(sieve);
}

/* ------------------------------------------------------------------*/
/* Main function required by the exercise – compute a(n)               */
seq_t a(idx_t n)
{
    /* Static storage for the sequence values */
    static seq_t *sequence = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static uint64_t next_candidate = 1;

    /* Initialise the list of primes once */
    init_primes();

    /* Generate terms until we have enough */
    while (seq_len < (size_t)n) {
        uint64_t k = next_candidate;
        seq_t ds_k = digit_sum_uint64(k);

        bool ok = true;
        uint64_t temp = k;

        /* Test all distinct prime divisors of k */
        for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
            uint32_t p = primes[i];
            if (temp % p == 0) {
                seq_t ds_m = digit_sum_uint64(k / p);
                if (ds_m != ds_k) {
                    ok = false;
                    break;
                }
                while (temp % p == 0)
                    temp /= p;
            }
        }

        if (ok && temp > 1) {          /* remaining prime factor */
            seq_t ds_m = digit_sum_uint64(k / temp);
            if (ds_m != ds_k)
                ok = false;
        }

        /* If k is a valid term, store it */
        if (ok) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(sequence, seq_cap * sizeof(seq_t));
                if (!tmp) {            /* allocation failure – abort */
                    return 0;
                }
                sequence = tmp;
            }
            sequence[seq_len++] = (seq_t)k;
        }

        ++next_candidate;
    }

    return sequence[(size_t)n - 1];
}
