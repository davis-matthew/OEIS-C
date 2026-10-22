/*
 * A369663  Numbers k of the form 4m+3, whose arithmetic derivative k' is of
 *          the form 4u+2, and k' has an even number of prime factors.
 * https://oeis.org/A369663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 938, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function to compute OEIS sequence A369663 (Numbers k of the form 4m+3
 * whose arithmetic derivative k' is of the form 4u+2 and k' has an even
 * number of prime factors).  The function generates candidates
 * in increasing order and keeps them in a static array. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;

/* Simple sieve up to LIMIT (inclusive).  LIMIT is chosen large enough
 * to cover all possible square‑roots of k for n ≤ 10000. */
static void sieve_primes(uint32_t limit)
{
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return;
    for (uint32_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) { free(is_prime); return; }
                primes = tmp; primes_cap = new_cap;
            }
            primes[primes_cnt++] = i;
        }
    free(is_prime);
}

/* Arithmetic derivative of a uint64_t number.  Returns the value as
 * uint64_t (the derivative is always smaller than the number). */
static uint64_t arithmetic_derivative_u64(uint64_t k)
{
    uint64_t sum = 0;
    uint64_t n = k;
    for (size_t i = 0; i < primes_cnt && primes[i] <= n / primes[i]; ++i) {
        uint32_t p = primes[i];
        if (n % p != 0) continue;
        unsigned exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        sum += (uint64_t)exp * (k / p);
    }
    if (n > 1) sum += k / n;  /* remaining prime factor */
    return sum;
}

/* Count prime factors (with multiplicity) of a uint64_t number. */
static unsigned count_prime_factors_u64(uint64_t m)
{
    unsigned cnt = 0;
    uint64_t n = m;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
        if (n == 1) return cnt;
    }
    if (n > 1) ++cnt;  /* remaining prime factor */
    return cnt;
}

/* Static storage of found terms (as uint64_t). */
static uint64_t *terms_u64 = NULL;
static size_t terms_cap = 0, terms_size = 0;
static uint64_t next_candidate = 3;   /* first k ≡ 3 (mod 4) */
static bool initialized = false;

/* Ensure primes are built and terms array is allocated to hold at least
 * 'need' entries. */
static void init_and_ensure(size_t need)
{
    if (!initialized) {
        sieve_primes(2000000);  /* enough for all k up to 10^12 */
        initialized = true;
    }
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(terms_u64, new_cap * sizeof(uint64_t));
        if (!tmp) { free(terms_u64); return; }
        terms_u64 = tmp; terms_cap = new_cap;
    }
}

/* Return the nth term of A369663 (1‑based indexing).  For n == 0 the
 * function returns 0 as a safeguard. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_and_ensure((size_t)n);
    while (terms_size < (size_t)n) {
        uint64_t k = next_candidate;
        next_candidate += 4;   /* keep k ≡ 3 (mod 4) */

        uint64_t d = arithmetic_derivative_u64(k);
        if ((d & 3) != 2) continue;          /* d ≡ 2 (mod 4) */
        if ((count_prime_factors_u64(d) & 1) != 0) continue;  /* Ω(d) even */

        if (terms_size == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            uint64_t *tmp = (uint64_t *)realloc(terms_u64, new_cap * sizeof(uint64_t));
            if (!tmp) { free(terms_u64); return (seq_t)0; }
            terms_u64 = tmp; terms_cap = new_cap;
        }
        terms_u64[terms_size++] = k;
    }
    return (seq_t)terms_u64[n - 1];
}
