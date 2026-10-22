/*
 * A352657  The number of lozenge tilings of a semiregular hexagon of side
 *          lengths n, n, 3*n, n, n and 3*n; equivalently, the number of plane
 *          partitions whose solid Young diagram fits inside an n X n X 3*n
 *          box.
 * https://oeis.org/A352657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = S(n)^2 * S(3*n) * S(5*n) / ( S(2*n) * S(4*n)^2 ), where
 *            S(m) = ∏_{k=0}^{m-1} k! (superfactorial)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   exponent_in_S, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_pow.h"
#include "../utils/exponent_in_S.h"

static uint64_t *primes = NULL;
static size_t    primes_count = 0;
static uint64_t  primes_limit = 0;

static void ensure_primes(uint64_t limit)
{
    if (limit <= primes_limit) return;
    if (primes) {
        free(primes);
        primes = NULL;
        primes_count = 0;
    }
    if (limit < 2) {
        primes_limit = limit;
        return;
    }
    size_t sz = (size_t)(limit + 1);
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (size_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            uint64_t step = (uint64_t)i;
            for (uint64_t j = i * i; j <= limit; j += step)
                is_prime[(size_t)j] = false;
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;
    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;
    free(is_prime);
    primes_limit = limit;
    primes_count = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    uint64_t max_needed = 5ULL * (uint64_t)n;
    ensure_primes(max_needed);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        unsigned long long e = 0ULL;
        e += 2ULL * exponent_in_S(p, (uint64_t)n);
        e += exponent_in_S(p, 3ULL * (uint64_t)n);
        e += exponent_in_S(p, 5ULL * (uint64_t)n);
        e -= exponent_in_S(p, 2ULL * (uint64_t)n);
        e -= 2ULL * exponent_in_S(p, 4ULL * (uint64_t)n);
        if (e == 0ULL) continue;
        seq_t pw = seq_pow((seq_t)p, (idx_t)e);
        result *= pw;
    }
    return result;
}
