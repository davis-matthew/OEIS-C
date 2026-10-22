/*
 * A347102  Totally additive with a(prime(k)) = A001223(k), where A001223
 *          gives the distance from the k-th prime to the next larger prime.
 * https://oeis.org/A347102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e||n} e * (next_prime(p)-p) (additive, a(p_k)=gap to
 *            next prime)
 * Range:     u8 n <= 8428, u16 n <= 8428, u32 n <= 8428, u64 n <= 8428,
 *            u128 n <= 8428, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* Helper: generate primes up to a fixed limit (sieve of Eratosthenes) */
#define PRIME_SIEVE_LIMIT 20000   /* > max prime needed for n≤10⁴   */

static uint64_t primes[PRIME_SIEVE_LIMIT];
static size_t prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool sieve[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) primes[prime_count++] = i;
    }
    primes_initialized = true;
}

/* ------------------------------------------------------------------------- */
/* Simple primality test using the pre‑computed prime table */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    init_primes();
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/* Next prime greater than given n (for small n, works fast) */
static uint64_t next_prime(uint64_t n)
{
    if (n < 2) return 2;
    uint64_t candidate = n + 1;
    if (candidate % 2 == 0) ++candidate;   /* keep odd */
    for (; ; candidate += 2) {
        if (is_prime(candidate)) return candidate;
    }
}

/* ------------------------------------------------------------------------- */
/* Completely additive function a(n) using factorisation and prime gaps */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for 0, return 0 just in case */
    init_primes();

    seq_t sum = 0;
    uint64_t m = (uint64_t)n;         /* work on uint64_t copy */

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            uint64_t gap = next_prime(p) - p;
            sum += (seq_t)(exp) * (seq_t)(gap);
        }
    }
    /* if remaining part is prime */
    if (m > 1) {
        uint64_t gap = next_prime(m) - m;
        sum += (seq_t)1 * (seq_t)(gap);
    }

    return sum;
}
