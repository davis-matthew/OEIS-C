/*
 * A369904  a(n) = n * Sum_{p|n, p prime} sigma(n/p) / p.
 * https://oeis.org/A369904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n} (n/p) * sigma(n/p)
 * Range:     u8 n <= 23, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(m) = sum of all positive divisors of m. */
/* a(1) = 0. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve helper ---------- */
static bool primes_ready = false;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

static void ensure_primes(uint64_t limit)
{
    /* If we already have enough primes, skip. */
    if (primes_ready && prime_count > 0 && prime_list[prime_count - 1] >= limit)
        return;

    size_t n = (size_t)limit + 1;
    bool *composite = (bool *)calloc(n, sizeof(bool));
    if (prime_list != NULL)
        free(prime_list);
    prime_list = (uint64_t *)malloc(n * sizeof(uint64_t));
    prime_count = 0;

    for (size_t i = 2; i < n; ++i) {
        if (!composite[i]) {
            prime_list[prime_count++] = i;
            if (i * i < n) {
                for (size_t j = i * i; j < n; j += i)
                    composite[j] = true;
            }
        }
    }
    free(composite);
    primes_ready = true;
}

/* ---------- Divisor sum sigma(m) ---------- */
static seq_t sigma_of_uint64(uint64_t m)
{
    if (!primes_ready)
        ensure_primes(m);
    seq_t result = (seq_t)1;
    uint64_t t = m;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= t; ++i) {
        uint64_t p = prime_list[i];
        if (t % p == 0) {
            seq_t pow = (seq_t)1;
            seq_t sum = (seq_t)1;   /* p^0 */

            while (t % p == 0) {
                t /= p;
                pow *= (seq_t)p;
                sum += pow;         /* add p^1 … p^e */
            }
            result *= sum;
        }
    }
    if (t > 1) {            /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)t);
    }
    return result;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;  /* a(1) = 0 */

    ensure_primes((uint64_t)n);

    seq_t sum = (seq_t)0;
    uint64_t t = (uint64_t)n;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= t; ++i) {
        uint64_t p = prime_list[i];
        if (t % p == 0) {
            uint64_t m = n / p;                     /* n/p */
            seq_t sig_m = sigma_of_uint64(m);
            seq_t term = (seq_t)(n / p) * sig_m;     /* (n/p) * sigma(n/p) */
            sum += term;
            while (t % p == 0)
                t /= p;                              /* remove p's powers */
        }
    }
    if (t > 1) {     /* remaining prime factor */
        uint64_t m = n / t;
        seq_t sig_m = sigma_of_uint64(m);
        seq_t term = (seq_t)(n / t) * sig_m;
        sum += term;
    }

    return sum;
}
