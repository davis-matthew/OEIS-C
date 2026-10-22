/*
 * A304818  If n = Product_i p(y_i) where p(i) is the i-th prime number and
 *          y_i <= y_j for i < j, then a(n) = Sum_i y_i*i.
 * https://oeis.org/A304818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{j=1}^{k} y_j * j, where y_j are the prime indices of
 *            the prime factorization of n sorted in nondecreasing order
 * Range:     u8 n <= 1437, u16 n <= 9574, u32 n <= 9574, u64 n <= 9574,
 *            u128 n <= 9574, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/prime_pi.h"   /* provides seq_t prime_pi(idx_t) */

/* static data for the list of small primes */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_initialized = false;

/* generate all primes up to LIMIT with a simple sieve */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    const idx_t LIMIT = 1000000;   /* enough for sqrt(n) up to ~10^12 */
    idx_t *is_prime = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));
    if (!is_prime)
        abort();

    for (idx_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++cnt;

    primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!primes)
        abort();

    size_t k = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[k++] = i;

    primes_cnt = cnt;
    primes_initialized = true;
    free(is_prime);
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    init_primes();

    idx_t tmp = n;
    seq_t sum = 0;
    idx_t pos = 1;          /* 1‑based position in the sorted list of indices */

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        idx_t p = primes[i];
        if (tmp % p != 0)
            continue;

        idx_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }

        idx_t prime_index = (idx_t)(i + 1);   /* primes[0] is the 1st prime */
        for (idx_t e = 0; e < exp; ++e) {
            sum += (seq_t)prime_index * (seq_t)pos;
            ++pos;
        }
    }

    if (tmp > 1) { /* tmp itself is a prime > sqrt(original n) */
        idx_t prime_index = (idx_t)prime_pi(tmp);   /* 1‑based index of tmp */
        sum += (seq_t)prime_index * (seq_t)pos;
    }

    return sum;
}
