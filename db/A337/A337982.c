/*
 * A337982  Number of pairs of divisors of 2n, (d1,d2) such that d1 < d2 and
 *          d1 + d2 is prime.
 * https://oeis.org/A337982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of divisors (d1,d2) of 2*n with d1 < d2
 *            and d1+d2 prime
 * Range:     u8 n <= 85, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple sieve to generate primes up to a fixed limit once */
#define PRIME_LIMIT 40000
#define MAX_PRIME_COUNT 5000

static bool primes_initialized = false;
static uint32_t primes[MAX_PRIME_COUNT];
static size_t prime_count = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool mark[PRIME_LIMIT + 1] = { false };
    primes[0] = 2; prime_count = 1;
    for (size_t i = 3; i <= PRIME_LIMIT; i += 2) mark[i] = true;
    for (size_t i = 3; i * i <= PRIME_LIMIT; i += 2) {
        if (mark[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i * 2) mark[j] = false;
        }
    }
    for (size_t i = 3; i <= PRIME_LIMIT; i += 2) {
        if (mark[i]) primes[prime_count++] = (uint32_t)i;
    }
    primes_initialized = true;
}

/* Check primality of a 64‑bit integer using the precomputed primes */
static bool is_prime_u64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Generate all divisors of m (unsigned 64‑bit) into an array */
static seq_t *divisors_of(uint64_t m, size_t *out_cnt)
{
    /* Factorization */
    uint64_t vals[64];
    int exps[64];
    size_t fcnt = 0;
    uint64_t temp = m;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            vals[fcnt] = p;
            exps[fcnt] = e;
            ++fcnt;
        }
    }
    if (temp > 1) {
        vals[fcnt] = temp;
        exps[fcnt] = 1;
        ++fcnt;
    }

    /* Generate divisors */
    size_t max_divs = 64;            /* ample for m <= 2*10^4 */
    seq_t *divs = (seq_t *)malloc(sizeof(seq_t) * max_divs);
    size_t dcnt = 1;
    divs[0] = (seq_t)1;

    for (size_t f = 0; f < fcnt; ++f) {
        uint64_t p = vals[f];
        int e = exps[f];
        size_t old = dcnt;
        seq_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t i = 0; i < old; ++i) {
                divs[dcnt++] = divs[i] * p_pow;
            }
        }
    }
    *out_cnt = dcnt;
    return divs;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of offset, harmless */
    init_primes();

    uint64_t m = (uint64_t)n * 2u;          /* 2*n */
    size_t dcnt;
    seq_t *divs = divisors_of(m, &dcnt);

    seq_t result = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        for (size_t j = i + 1; j < dcnt; ++j) {
            seq_t sum = divs[i] + divs[j];
            if (is_prime_u64((uint64_t)sum)) {
                ++result;
            }
        }
    }
    free(divs);
    return result;
}
