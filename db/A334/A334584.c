/*
 * A334584  Perimeters of primitive Heronian triangles whose smallest side
 *          length is prime.
 * https://oeis.org/A334584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = perimeter of the nth primitive Heronian triangle whose
 *            shortest side is prime (computed by search)
 * Range:     u8 n <= 56, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   area_integer, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/area_integer.h"
#include "../utils/gcd_u64.h"

/* Simple GCD for 64‑bit integers. */

/* Prime sieve up to a fixed limit (chosen large enough for n≈10000). */
static uint32_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count   = 0;      /* number of primes */

static void init_primes(void)
{
    if (prime_list) return;   /* already done */

    const uint32_t LIMIT = 800000u;   /* safe upper bound */

    /* Allocate and initialize boolean array. */
    bool *is_prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(1);
    memset(is_prime, true, LIMIT + 1);
    is_prime[0] = is_prime[1] = false;

    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    /* Store primes. */
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);
    size_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[pos++] = i;

    prime_count = cnt;
    free(is_prime);
}

/* Cache of already found perimeters; values are stored as 64‑bit
   integers to keep allocation trivial. */
static uint64_t *cache     = NULL;
static size_t    cache_sz  = 0;
static size_t    cache_cap = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure primes are ready. */
    init_primes();

    /* Initialise cache lazily. */
    if (!cache) {
        cache_cap = 64;
        cache = (uint64_t *)calloc(cache_cap, sizeof(uint64_t));
        if (!cache) exit(1);
    }

    static uint64_t last_perim = 12;   /* next perimeter to test */

    /* Produce entries up to requested index. */
    while (cache_sz < (size_t)n) {
        uint64_t p = last_perim++;
        bool found = false;
        uint64_t a_max = p / 3;                     /* smallest side ≤ p/3 */

        /* Iterate over primes a. */
        for (size_t pi = 0; pi < prime_count && prime_list[pi] <= a_max; ++pi) {
            uint64_t a = prime_list[pi];

            /* b ranges from max(a, p/2 - a + 1) to (p-a)/2. */
            uint64_t min_b = (p / 2) - a + 1;
            if (min_b < a) min_b = a;
            uint64_t max_b = (p - a) / 2;
            if (min_b > max_b) continue;

            for (uint64_t b = min_b; b <= max_b; ++b) {
                uint64_t c = p - a - b;
                /* Primitive test. */
                uint64_t g = gcd_u64(a, b);
                g = gcd_u64(g, c);
                if (g != 1) continue;

                /* Integer area test. */
                if (area_integer(p, a, b)) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (found) {
            if (cache_sz >= cache_cap) {            /* grow cache */
                cache_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_sz++] = p;
        }
    }

    return (seq_t)cache[n - 1];
}
