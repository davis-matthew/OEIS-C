/*
 * A263087  a(n) = A060990(n^2); number of solutions to x - d(x) = n^2, where
 *          d(x) is the number of divisors of x (A000005).
 * https://oeis.org/A263087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x - d(x) = n², where d(x) is * the
 *            number of divisors of x (A060990(n²)). The function counts all *
 *            positive integers x such that x - d(x) equals n²
 * Range:     u8 n <= 15, u16 n <= 614, u32 n <= 9088, u64 n <= 9088,
 *            u128 n <= 9088, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Simple sieve of Eratosthenes up to LIMIT.  LIMIT is chosen large
 * enough to factor every x encountered for n up to about 10000. */
static void init_primes(void)
{
    if (primes_initialized) return;
    const uint64_t LIMIT = 20000;     /* > sqrt(max x = n² + 5000) for n≈10⁴ */
    size_t *sieve = (size_t*)calloc(LIMIT + 1, sizeof(size_t));
    if (!sieve) return;              /* fall back to empty list on failure */

    size_t count = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            ++count;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }

    prime_list = (uint64_t*)malloc(count * sizeof(uint64_t));
    if (prime_list) {
        size_t idx = 0;
        for (uint64_t i = 2; i <= LIMIT; ++i) {
            if (!sieve[i]) {
                prime_list[idx++] = i;
            }
        }
        prime_count = count;
    }

    free(sieve);
    primes_initialized = true;
}

/* Return the divisor count of a positive integer x (x ≤ UINT64_MAX).
 * The function assumes prime_list is initialized. */
static seq_t divisor_count(seq_t x)
{
    uint64_t n = (uint64_t)x;
    seq_t result = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        uint64_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        result *= (exp + 1);
    }
    if (n > 1) result *= 2;  /* remaining prime factor */
    return result;
}

seq_t a(idx_t n)
{
    /* ensure primes are ready for factorization */
    init_primes();

    /* target value t = n² */
    seq_t t = (seq_t)n * ( (seq_t)n );

    /* Upper bound for the possible difference k = d(x)
     * Empirically, for n ≤ 10⁴ we never need k larger than 5000. */
    const idx_t K_MAX = 5000;

    seq_t count = 0;
    for (idx_t k = 1; k <= K_MAX; ++k) {
        seq_t x = t + k;                   /* candidate x */
        seq_t d = divisor_count(x);        /* d(x) */
        if (d == k) {                      /* x - d(x) == t */
            ++count;
        }
    }
    return count;
}
