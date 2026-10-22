/*
 * A305212  a(n) = n - A305211(n).
 * https://oeis.org/A305212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - B(n) * where B(n) = number of residues modulo n that
 *            can be written as the sum * of two integer cubes. B(n) is
 *            multiplicative, and for a prime power * m = p^e we compute B(m)
 *            by brute force enumeration of the cubic * residues mod m. The
 *            function a(idx_t n) returns the sequence value *...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   compute_B_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/compute_B_prime_power.h"

/* Global cache of B(p^e) values indexed by the modulus m.  B_prime_power[m]
 * stores the number of representable residues modulo m, if it has been
 * computed.  A zero value means "not yet computed". */
static seq_t *B_prime_power = NULL;
static size_t B_size = 0;

/* List of primes used for factorisation.  The array is rebuilt
 * whenever a larger upper bound on primes is required. */
static int *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Resize the B_prime_power array to hold indices up to limit.  Newly
 * allocated entries are zero‑initialised. */
static void ensure_size(idx_t limit)
{
    if ((size_t)(limit + 1) <= B_size)
        return;
    size_t new_size = (size_t)(limit + 1);
    seq_t *new_B = (seq_t *)realloc(B_prime_power, new_size * sizeof(seq_t));
    if (!new_B)
        exit(EXIT_FAILURE);
    if (B_prime_power) { /* existing entries are now in new_B */
        B_prime_power = new_B;
    } else {
        B_prime_power = new_B;
    }
    memset(B_prime_power + B_size, 0, (new_size - B_size) * sizeof(seq_t));
    B_size = new_size;
}

/* Sieve of Eratosthenes to generate all primes up to limit.  The existing
 * list is discarded and rebuilt. */
static void generate_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return; /* already generated */
    if (primes) {
        free(primes);
        primes = NULL;
    }

    int n = (int)limit;
    bool *is_comp = (bool *)calloc(n + 1, sizeof(bool));
    if (!is_comp)
        exit(EXIT_FAILURE);

    for (int i = 2; i * i <= n; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= n; j += i)
                is_comp[j] = true;

    size_t cnt = 0;
    for (int i = 2; i <= n; ++i)
        if (!is_comp[i])
            ++cnt;

    int *new_primes = (int *)malloc(cnt * sizeof(int));
    if (!new_primes)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (int i = 2; i <= n; ++i)
        if (!is_comp[i])
            new_primes[idx++] = i;

    free(is_comp);
    primes = new_primes;
    primes_cnt = cnt;
    primes_limit = limit;
}

/* Ensure B_prime_power has values for all prime powers ≤ n. */
static void ensure_B_prime_powers(idx_t n)
{
    ensure_size(n);
    if (B_prime_power[1] == 0)
        B_prime_power[1] = (seq_t)1; /* R(1) = {0} */

    generate_primes(n);

    for (size_t i = 0; i < primes_cnt; ++i) {
        int p = primes[i];
        idx_t m = (idx_t)p;
        while (m <= n) {
            if (B_prime_power[m] == 0)
                B_prime_power[m] = compute_B_prime_power(m);
            if (m > n / (idx_t)p) /* avoid overflow */
                break;
            m *= (idx_t)p;
        }
    }
}

/* Multiply the B-values of every prime power dividing n.  The product
 * never exceeds n and fits comfortably in idx_t. */
static idx_t product_B(idx_t n)
{
    idx_t prod = 1;
    idx_t tmp = n;
    generate_primes(n);

    for (size_t i = 0; i < primes_cnt &&
                     (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        idx_t p = (idx_t)primes[i];
        if (tmp % p == 0) {
            idx_t m = 1;
            while (tmp % p == 0) {
                tmp /= p;
                m *= p;
            }
            prod *= (idx_t)B_prime_power[m];
        }
    }
    if (tmp > 1) /* remaining prime factor */
        prod *= (idx_t)B_prime_power[tmp];

    return prod;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* safety for out-of-bounds calls */
    ensure_B_prime_powers(n);

    idx_t prod = product_B(n);
    seq_t result = (seq_t)((idx_t)n - prod);
    return result;
}
