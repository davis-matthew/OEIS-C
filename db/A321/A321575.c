/*
 * A321575  Nexus primary pretenders: a(n) is the smallest composite k such
 *          that n^k - (n-1)^k == 1 (mod k).
 * https://oeis.org/A321575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   smallest composite k ≤ 561 such that n^k - (n-1)^k ≡ 1 (mod k)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small_u64.h"

/* Compute b^e (mod m) for small modulus m ≤ 561.  Operates on uint64_t
   to avoid overflow, then casts the final result to seq_t.  The
   intermediate products stay below 64 bit. */
static uint64_t mod_pow_uint64(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t res = 1 % m;
    uint64_t base = b % m;
    while (e) {
        if (e & 1ULL) res = (res * base) % m;
        e >>= 1ULL;
        if (e) base = (base * base) % m;
    }
    return res;
}

/* Pre‑computed list of composite numbers up to 561. */
static bool composites_ready = false;
static uint64_t composites[600];
static size_t composite_count;

/* Initialise the composites array the first time it is needed. */
static void init_composites(void)
{
    if (composites_ready) return;
    size_t idx = 0;
    for (uint64_t k = 4; k <= 561; ++k) {
        if (!is_prime_small_u64(k)) composites[idx++] = k;
    }
    composite_count = idx;
    composites_ready = true;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    /* Known values for residues other than 2 (mod 6). */
    uint64_t r6 = n % 6;
    if (r6 == 1 || r6 == 3 || r6 == 5) return (seq_t)4;
    if (r6 == 4) return (seq_t)6;
    if (r6 == 0) return (seq_t)9;

    /* n ≡ 2 (mod 6): search among composites. */
    init_composites();
    for (size_t i = 0; i < composite_count; ++i) {
        uint64_t k = composites[i];
        uint64_t nhk   = mod_pow_uint64(n      , k, k);
        uint64_t nhm1k = mod_pow_uint64(n - 1ULL, k, k);
        uint64_t diff = (nhk + k - nhm1k) % k;
        if (diff == 1ULL) return (seq_t)k;
    }
    /* Theoretically unreachable. */
    return (seq_t)0;
}
