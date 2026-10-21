/*
 * A244771  Prime numbers ending in the prime number 53.
 * https://oeis.org/A244771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A244771 – primes of the form 100*k + 53 (k ≥ 0)
 *
 * The function a(n) returns the n-th such prime (n ≥ 1).
 * No pre‑computed table or auxiliary allocation is required.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* deterministic Miller–Rabin primality test for 64‑bit integers */

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return r;
}
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
                                    23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,
                                     450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (!a) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* sequence implementation -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below the offset */
    idx_t found = 0;
    uint64_t k = 0;
    while (found < n) {
        uint64_t cand = 53ULL + 100ULL * k;
        if (is_prime_uint64(cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
        ++k;
    }
    /* unreachable but keeps the compiler happy */
    return 0;
}
