/*
 * A361796  Prime numbers preceded by two consecutive numbers which are
 *          products of four distinct primes (or tetraprimes).
 * https://oeis.org/A361796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p such that p−1 and p−2 are products of
 *            exactly four distinct primes (tetraprimes). The function
 *            searches for primes starting at 2 until the n‑th such prime is
 *            found. Indexed as in OEIS: a(1) = 8647
 * Range:     u8 none, u16 n <= 29, u32 n <= 1914, u64 n <= 1914,
 *            u128 n <= 1914, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* multiply modulo 2^64 – used only for 64‑bit prime testing */

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin bases for 64‑bit integers */
static const uint64_t mr_bases[] = {
    2ULL, 325ULL, 9375ULL, 28178ULL,
    450775ULL, 9780504ULL, 1795265022ULL
};

/* primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    static const uint32_t small[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < sizeof(mr_bases)/sizeof(mr_bases[0]); ++i) {
        uint64_t a = mr_bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- prime sieve for trial division ---------- */

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_prime_list(void)
{
    if (prime_list) return;

    const uint64_t LIMIT = 100000;        /* > sqrt(max product to test) */
    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve) return;   /* unlikely */

    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;

    /* count primes */
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) ++prime_count;

    prime_list = (uint64_t *)calloc(prime_count, sizeof(uint64_t));
    if (!prime_list) {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = i;
    free(sieve);
}

/* factor a 64‑bit integer, counting distinct and total factors */
static void factor_uint64(uint64_t n, int *distinct, int *total)
{
    *distinct = 0;
    *total = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            ++(*distinct);
            while (n % p == 0) {
                n /= p;
                ++(*total);
            }
        }
    }
    if (n > 1) {
        ++(*distinct);
        ++(*total);
    }
}

/* check that m is a product of exactly four distinct primes */
static bool is_tetraprime_uint64(uint64_t m)
{
    if (m < 2) return false;
    int distinct = 0, total = 0;
    factor_uint64(m, &distinct, &total);
    return distinct == 4 && total == 4;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard for underflow */

    init_prime_list();             /* ensure primes are ready */

    idx_t found = 0;
    uint64_t cand = 2;              /* start from 2 */

    while (found < n) {
        if (is_prime_uint64(cand)) {
            uint64_t a1 = cand - 1;
            uint64_t a2 = cand - 2;
            if (is_tetraprime_uint64(a1) && is_tetraprime_uint64(a2)) {
                ++found;
                if (found == n) return (seq_t)cand;
            }
        }
        /* skip even numbers after 2 */
        if (cand == 2) cand = 3;
        else cand += 2;
    }
    return (seq_t)cand;   /* never reached */
}
