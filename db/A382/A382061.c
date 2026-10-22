/*
 * A382061  Numbers whose number of divisors is divisible by their number of
 *          unitary divisors.
 * https://oeis.org/A382061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A382061: numbers k such that the number of unitary divisors
 *  of k divides the total number of divisors of k.  Computed by
 *  sequentially testing integers and caching results.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------- Prime sieve -------------------------------- */
#define PRIME_LIMIT 1000000                 /* primes up to 1 e6 are enough for n ≤ 10 000 */

static uint32_t *primes = NULL;            /* array of primes */
static size_t prime_cnt = 0;               /* number of primes stored */

/* initialise the prime array once */
static void init_primes(void)
{
    if (prime_cnt != 0) return;           /* already initialised */

    /* sieve of Eratosthenes */
    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    /* count primes first for allocation */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;

    prime_cnt = cnt;
    free(is_comp);
}

/* --------------------- Property test ---------------------------- */
static bool good(uint64_t n)
{
    if (n == 1) return true;              /* d_u(1) = d(1) = 1 */

    uint64_t tmp = n;
    size_t omega = 0;                     /* #distinct prime factors */
    int sum_v2 = 0;                       /* total exponent of 2 in ∏(e_i+1) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > tmp) break;
        if (tmp % p == 0) {
            ++omega;
            int e = 0;
            do {
                ++e;
                tmp /= p;
            } while (tmp % p == 0);
            int val = e + 1;
            while ((val & 1) == 0) {
                val >>= 1;
                ++sum_v2;
            }
        }
    }

    if (tmp > 1) {            /* remaining prime factor */
        ++omega;
        ++sum_v2;              /* e = 1 → e+1 = 2 → v₂ = 1 */
    }

    return sum_v2 >= (int)omega;
}

/* --------------------- Cache of results ------------------------ */
static seq_t *cache = NULL;           /* cached terms */
static size_t cache_len = 0;          /* number of cached terms */
static size_t cache_cap = 0;          /* capacity of cache array */
static uint64_t tested = 1;           /* next integer to test */

/* --------------------- The required function -------------------- */
seq_t a(idx_t n)
{
    /* initialise primes first */
    init_primes();

    /* ensure cache capacity */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (good(tested))
            cache[cache_len++] = (seq_t)tested;
        ++tested;
    }

    return cache[n - 1];
}
