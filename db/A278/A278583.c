/*
 * A278583  Numbers k such that k+1 is a prime, k+2 is twice a prime, and k+3
 *          is three times a prime.
 * https://oeis.org/A278583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th number k (k ≥ 12) such that k+1 is a prime, k+2
 *            is twice a prime, and k+3 is three times a prime. All such k are
 *            divisible by 12. Thus k = 12·m and the conditions become:
 *            12·m+1, 6·m+1, and 4·m+1 are all prime. The function evaluates
 *            this definition by a brute‑force search, cachin...
 * Range:     u8 n <= 3, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small deterministic Miller–Rabin for 64‑bit integers (handles all
   unsigned 64‑bit values).  Uses 6 bases which are adequate for
   n < 3·10^18.  For the ranges required here it is fully deterministic. */
static inline uint64_t mul64_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1U) r = mul64_mod(r, a, mod);
        a = mul64_mod(a, a, mod);
        e >>= 1U;
    }
    return r;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }
    static const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1U || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul64_mod(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Sequence generator ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1, values for n<1 are irrelevant */

    /* static cache of generated terms */
    static seq_t *terms   = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_m = 1;  /* next m to examine */

    /* generate terms until we have at least n entries */
    while (terms_len < (size_t)n) {
        /* ensure enough space */
        if (terms_cap == 0 || terms_len == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 128;
            if (new_cap < terms_len + 1) new_cap = terms_len + 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) abort();                 /* out‑of‑memory */
            terms = tmp;
            terms_cap = new_cap;
        }

        /* find next admissible m */
        for (;;) {
            uint64_t m = next_m++;
            uint64_t p1 = 12ULL * m + 1ULL;
            if (!is_prime64(p1)) continue;
            uint64_t p2 = 6ULL * m + 1ULL;
            if (!is_prime64(p2)) continue;
            uint64_t p3 = 4ULL * m + 1ULL;
            if (!is_prime64(p3)) continue;

            terms[terms_len++] = (seq_t)(12ULL * m);
            break;
        }
    }
    return terms[n - 1];
}
