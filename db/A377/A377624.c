/*
 * A377624  a(n) is the number of iterations of x -> 6*x + 5 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A377624:
   a(n) = the number of iterations of x -> 6*x + 5 until the numbers of
   primes and composites seen are equal, starting from prime(n). */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

/* ---- Miller–Rabin primality test for 64‑bit integers ---- */
static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_u64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t result = 1;
    while (d) {
        if (d & 1) result = mod_mul_u64(result, a, mod);
        a = mod_mul_u64(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;

    /* quick check for small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); i++) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; r++) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- Sequence implementation ---- */
seq_t a(idx_t n) {
    /* start from the nth prime */
    seq_t x = (seq_t)get_prime(n);

    idx_t primes   = 1; /* starting prime counts */
    idx_t composites = 0;
    idx_t steps = 0;

    while (primes != composites) {
        /* iterate */
        x = x * 6 + 5;
        steps++;

        /* test primality of the new value */
        uint64_t v = (uint64_t)x;   /* all values stay < 2^64 for n ≤ 10000 */
        if (is_prime_uint64(v))
            primes++;
        else
            composites++;
    }
    return (seq_t)steps;
}
