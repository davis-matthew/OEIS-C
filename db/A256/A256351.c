/*
 * A256351  Composites in base 10 that remain composite in exactly seven bases
 *          b, 2 <= b <= 10, expansions interpreted as decimal numbers.
 * https://oeis.org/A256351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth composite number that is composite in exactly
 *            seven * bases b, 2 <= b <= 10, when its base‑b representation is
 *            interpreted as * a decimal number
 * Range:     u8 n <= 29, u16 n <= 4159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The implementation does not rely on any external helper functions
 *      except for the standard C library.  It contains its own deterministic
 *      64‑bit Miller–Rabin primality test.  All arithmetic on seq_t values
 *      is performed via conversions to/from uint64_t because the decimal
 *      numbers involved fit into 64 bits for the ranges required by the
 *      problem statement. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- deterministic 64‑bit Miller–Rabin primality test ---------- */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* small prime trial division */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Miller–Rabin with deterministic bases for 64‑bit integers. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- helper for dynamic array of seq_t ---------- */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t new_cap = *cap ? *cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next composite to consider */
    static uint64_t next_candidate = 4;   /* first composite > 1 */

    if (n == 0) return 0; /* undefined for index 0, but return 0 safely */

    while (cache_len < n) {
        /* find next composite number */
        while (is_prime_u64(next_candidate)) next_candidate++;

        uint64_t m = next_candidate;
        int comp_bases = 0;

        /* test bases 2 .. 10 */
        for (int b = 2; b <= 10; ++b) {
            /* digits of m in base b (least significant first) */
            int digits[64];
            int dlen = 0;
            uint64_t t = m;
            do {
                digits[dlen++] = t % b;
                t /= b;
            } while (t);

            /* interpret digits as a decimal number */
            uint64_t dec = 0;
            for (int i = dlen - 1; i >= 0; --i) {
                dec = dec * 10 + (uint64_t)digits[i];
            }

            if (dec > 1 && !is_prime_u64(dec)) ++comp_bases;
        }

        if (comp_bases == 7) {
            grow_seq_array(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
