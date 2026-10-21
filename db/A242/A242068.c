/*
 * A242068  First of two consecutive sphenic numbers with no semiprime between
 *          them.
 * https://oeis.org/A242068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5470, u32 n <= 8683, u64 n <= 8683,
 *            u128 n <= 8683, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence consists of the first of two
 *  consecutive sphenic numbers with no semiprime
 *  between them.  We generate the sequence by
 *  scanning the natural numbers, detecting sphenic
 *  numbers and testing the interval between two
 *  consecutive sphenic numbers for the presence of a
 *  semiprime.  The terms are memoised so arbitrary
 *  order of calls is supported. */

/* Include only the standard headers that work in
 * C11 and C++17. The arbitrary‑precision build
 * supplies its own definitions of seq_t and idx_t. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- Prime sieve --------- */
#define PRIME_LIMIT 2000000U          /* sieve size */

static uint32_t *primes = NULL;      /* array of primes */
static size_t  prime_cnt = 0;        /* number of primes found */

/* Initialise the sieve the first time we need primes. */
static void init_prime_sieve(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) exit(EXIT_FAILURE);

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                sieve[(size_t)j] = true;

    primes = (uint32_t *)malloc((PRIME_LIMIT / 10) * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t cap = PRIME_LIMIT / 10;
    prime_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[(size_t)i]) {
            if (prime_cnt == cap) {
                cap <<= 1;
                primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[prime_cnt++] = i;
        }
    free(sieve);
}

/* --------- Helper functions --------- */
static bool is_sphenic(uint64_t n)
{
    if (n < 30) return false;   /* 2*3*5 = 30 is the smallest sphenic */

    init_prime_sieve();
    uint64_t tmp = n;
    int distinct = 0;
    int total    = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > tmp) break;
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                ++total;
                if (total > 3) return false;           /* too many factors */
            }
            ++distinct;
            if (exp != 1) return false;                /* must be power 1 */
            if (distinct > 3) return false;
        }
    }
    if (tmp > 1) {           /* remaining prime factor */
        ++distinct;
        ++total;
    }
    return (distinct == 3) && (total == 3);
}

static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;   /* 2*2 = 4 is the smallest semiprime */

    init_prime_sieve();
    uint64_t tmp = n;
    int total = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > tmp) break;
        if (tmp % p == 0) {
            while (tmp % p == 0) {
                tmp /= p;
                ++total;
                if (total > 2) return false; /* more than two prime factors */
            }
        }
    }
    if (tmp > 1) ++total;     /* remaining prime factor */
    return total == 2;
}

/* --------- Cache‑management --------- */
static void ensure_cache_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    if (new_cap < need) new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    *arr = new_arr;
    *cap = new_cap;
}

/* --------- Main sequence function --------- */
seq_t a(idx_t n)
{
    static uint64_t last_checked   = 0;   /* highest integer examined */
    static uint64_t prev_sphenic   = 0;   /* previous sphenic number found */
    static seq_t  *cache           = NULL;   /* memoised terms */
    static size_t cache_cap        = 0;     /* capacity of cache */
    static size_t cache_size       = 0;     /* number of terms stored */

    if (n == 0) return 0; /* undefined for n < 1, but not used */

    /* Ensure we have found at least n terms. */
    while (cache_size < (size_t)n) {
        uint64_t candidate = last_checked + 1;
        if (is_sphenic(candidate)) {
            if (prev_sphenic != 0) {
                bool has_semiprime = false;
                for (uint64_t m = prev_sphenic + 1; m < candidate; ++m) {
                    if (is_semiprime(m)) { has_semiprime = true; break; }
                }
                if (!has_semiprime) {
                    ensure_cache_cap(&cache, &cache_cap, cache_size + 1);
                    cache[cache_size++] = (seq_t)prev_sphenic;
                }
            }
            prev_sphenic = candidate;
        }
        last_checked = candidate;
    }
    return cache[n - 1];
}
