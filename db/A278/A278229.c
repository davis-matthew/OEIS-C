/*
 * A278229  Least number with the prime signature of 2*prime(n) - 1.
 * https://oeis.org/A278229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number with the same prime signature as
 *            2*prime(n)-1
 * Range:     u8 n <= 269, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: generate all primes up to a limit using a simple sieve.
   The array 'global_primes' and its count are filled the first time
   this function is called.  It is large enough for n up to 10000. */
static const size_t PRIME_LIMIT = 500000; /* > 10 000th prime (104 729)   */
static uint32_t *global_primes = NULL;
static size_t global_prime_cnt = 0;

static void build_primes(void)
{
    if (global_primes != NULL)
        return; /* already built */

    /* Allocate buffer for prime candidates */
    bool *is_composite = (bool *)malloc(PRIME_LIMIT + 1);
    if (!is_composite) {
        /* In a real system we would handle the error, but for the
           purposes of the sequence computation we abort here. */
        exit(1);
    }
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_composite[i] = false;

    size_t cnt = 0;
    uint32_t *primes = (uint32_t *)malloc(PRIME_LIMIT * sizeof(uint32_t));
    if (!primes) {
        free(is_composite);
        exit(1);
    }

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    global_primes = primes;
    global_prime_cnt = cnt;
    free(is_composite);
}

/* Factorize n (n < PRIME_LIMIT*A000040) by trial division using the
   precomputed primes.  Results are stored in 'exps' and the count in
   '*cnt'.  The arrays must have enough room for the worst case (here
   16 elements is more than enough for numbers < 209 457). */
static void factor_small(uint64_t n, uint32_t exps[16], size_t *cnt)
{
    size_t fcnt = 0;
    uint64_t m = n;

    for (size_t i = 0; i < global_prime_cnt && (uint64_t)global_primes[i] * global_primes[i] <= m; ++i) {
        uint32_t p = global_primes[i];
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[fcnt++] = e;
        }
    }
    if (m > 1 && fcnt < 16) { /* m itself is prime */
        exps[fcnt++] = 1;
    }
    *cnt = fcnt;
}

/* Simple bubble sort to order exponents in descending order. */
static void sort_desc(uint32_t exps[16], size_t cnt)
{
    for (size_t i = 0; i + 1 < cnt; ++i) {
        for (size_t j = 0; j + 1 < cnt - i; ++j) {
            if (exps[j] < exps[j + 1]) {
                uint32_t tmp = exps[j];
                exps[j] = exps[j + 1];
                exps[j + 1] = tmp;
            }
        }
    }
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    /* Ensure prime table is ready. */
    if (!global_primes)
        build_primes();

    /* 1. Retrieve the n-th prime (primes array is 0‑based). */
    if (n == 0 || (size_t)n > global_prime_cnt) {
        /* Undefined for n beyond the precomputed range. */
        return (seq_t)0;
    }
    uint32_t p = global_primes[n - 1U];

    /* 2. Compute m = 2*p - 1. */
    uint64_t m = 2ULL * (uint64_t)p - 1ULL;

    /* 3. Factor m, collecting exponents. */
    uint32_t exps[16];
    size_t cnt = 0;
    factor_small(m, exps, &cnt);

    /* 4. Sort exponents so that the largest exponent gets the smallest prime. */
    sort_desc(exps, cnt);

    /* 5. Build the minimal number with the same signature. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t prime = (seq_t)global_primes[i];
        for (uint32_t e = 0; e < exps[i]; ++e)
            result *= prime;
    }

    return result;
}
