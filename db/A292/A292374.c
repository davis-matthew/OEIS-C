/*
 * A292374  a(1) = 1, a(2n) = 0, and for odd numbers n > 1, a(n) =
 *          a(A064989(n)) + [n == 1 (mod 4)].
 * https://oeis.org/A292374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3732, u16 n <= 8251, u32 n <= 8251, u64 n <= 8251,
 *            u128 n <= 8251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292374:
   a(1) = 1,
   a(2n) = 0,
   for odd n > 1: a(n) = a(A064989(n)) + [n == 1 (mod 4)]   (recurrence)
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- constants ---------- */
#define MAX_LIMIT  200000            /* maximum value for the prime sieve */
#define CACHE_SIZE 200000            /* maximum index to cache */

/* ---------- prime sieve ---------- */
static uint32_t prime_list[MAX_LIMIT + 1];
static size_t   prime_count = 0;
static bool     primes_built = false;

static void build_prime_sieve(void)
{
    if (primes_built) return;
    primes_built = true;

    bool *is_comp = (bool *)calloc(MAX_LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();

    for (size_t i = 2; i * i <= MAX_LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= MAX_LIMIT; j += i)
                is_comp[j] = true;

    for (size_t i = 2; i <= MAX_LIMIT; ++i)
        if (!is_comp[i]) prime_list[prime_count++] = (uint32_t)i;

    free(is_comp);
}

/* ---------- factorisation of odd n (n <= MAX_LIMIT) ---------- */
static void factor_odd(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_count &&
         (uint64_t)prime_list[i] * prime_list[i] <= n; ++i)
    {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pr[(*cnt)] = p;
            ex[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {                     /* n itself is prime */
        pr[(*cnt)] = n;
        ex[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ---------- predecessor of an odd prime (p > 2) ---------- */
static uint64_t predecessor_prime(uint64_t p)
{
    if (p == 3) return 2;
    size_t lo = 0, hi = prime_count - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (uint64_t)prime_list[lo - 1];
}

/* ---------- A064989 for an odd n (>1) ---------- */
static uint64_t map_A064989(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t   cnt;
    factor_odd(n, pr, ex, &cnt);

    uint64_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pred = predecessor_prime(pr[i]);
        for (uint64_t e = ex[i]; e > 0; --e) res *= pred;
    }
    return res;
}

/* ---------- cache for a(n) ---------- */
static seq_t   *cache      = NULL;
static unsigned char *cached = NULL;
static bool    cache_built = false;

static void init_cache(void)
{
    if (cache_built) return;
    cache_built = true;

    cache   = (seq_t *)calloc(CACHE_SIZE + 1, sizeof(seq_t));
    cached  = (unsigned char *)calloc(CACHE_SIZE + 1, sizeof(unsigned char));
    if (!cache || !cached) abort();

    build_prime_sieve();
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    init_cache();          /* ensure cache and primes are ready */

    /* base cases */
    if (n == 1) return (seq_t)1;
    if ((n & 1) == 0) return (seq_t)0;

    /* use cached value if available */
    if (n <= (idx_t)CACHE_SIZE && cached[n]) return cache[n];

    /* compute recursively */
    uint64_t mapped = map_A064989((uint64_t)n);
    seq_t    val = a((idx_t)mapped) + (((n & 3) == 1) ? (seq_t)1 : (seq_t)0);

    /* store in cache if within bounds */
    if (n <= (idx_t)CACHE_SIZE) {
        cached[n] = 1;
        cache[n]  = val;
    }
    return val;
}
