/*
 * A386027  Primes having only {0, 1, 4, 5} as digits.
 * https://oeis.org/A386027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th prime whose decimal digits belong to {0,1,4,5}.
   The values are generated on demand by a simple search and stored
   for subsequent calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/*  64‑bit modular multiplication and exponentiation (used by primality test)  */
#if defined(__SIZEOF_INT128__)
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}
#else
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t res = 0;
    while (b) {
        if (b & 1) {
            res = res + a >= mod ? res + a - mod : res + a;
        }
        a = a + a >= mod ? a + a - mod : a + a;
        b >>= 1;
    }
    return res;
}
#endif

static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul64(r, a, mod);
        a = mod_mul64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller‑Rabin test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------- */
/*  Storage for primes found so far (64‑bit values)  */
static uint64_t *primes64 = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Grow the primes buffer when necessary. */
static void primes_grow(void)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 256;
        uint64_t *tmp = (uint64_t *)realloc(primes64, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes64 = tmp;
        primes_cap = new_cap;
    }
}

/* Store a new prime value. */
static void add_prime(uint64_t val)
{
    primes_grow();
    primes64[primes_len++] = val;
}

/* ------------------------------------------------------------------- */
/*  Recursive generation of numbers of a fixed length  */
static void generate_len_rec(size_t len, size_t depth, uint64_t val)
{
    if (depth == len - 2) {            /* all middle digits placed */
        uint64_t candidate = val * 10 + 1;
        if (is_prime_uint64(candidate))
            add_prime(candidate);
        return;
    }
    const int mids[4] = {0, 1, 4, 5};
    for (int i = 0; i < 4; ++i) {
        generate_len_rec(len, depth + 1, val * 10 + mids[i]);
    }
}

/* Generate primes for all numbers of the given length. */
static void generate_len(size_t len)
{
    if (len == 1) {                    /* only number is "5" */
        if (is_prime_uint64(5ULL))
            add_prime(5ULL);
        return;
    }
    const int leads[3] = {1, 4, 5};
    for (int i = 0; i < 3; ++i) {
        generate_len_rec(len, 0, (uint64_t)leads[i]);
    }
}

/* ------------------------------------------------------------------- */
/*  Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below the offset */

    /* Ensure we have at least n primes, up to a reasonable maximum length. */
    static size_t gen_len = 1;          /* length already processed */
    const size_t MAX_LEN = 12;          /* sufficient for n up to several 10k */

    while (primes_len < (size_t)n && gen_len <= MAX_LEN) {
        generate_len(gen_len);
        ++gen_len;
    }

    if (primes_len >= (size_t)n)
        return (seq_t)primes64[n - 1];

    /* If we ran out of generated numbers (unlikely for the tested range) */
    return 0;
}
