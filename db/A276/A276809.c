/*
 * A276809  Least prime p such that (p^2-1)/24 is divisible by prime(n) or 0
 *          if no such prime exists.
 * https://oeis.org/A276809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p ≥ 5 such that (p^2-1)/24 is divisible by
 *            prime(n)
 * Range:     u8 n <= 14, u16 n <= 421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If no such prime exists the function returns 0. */
/* The function uses the provided utility functions:
 *   uint32_t get_prime(idx_t k) – kth prime (k ≥ 1)
 *   uint64_t next_prime(uint64_t start) – next prime ≥ start
 * It performs a simple search over successive primes until the condition is met.
 * The test (p^2-1)/24 divisible by prime(n) is equivalent to (p^2 mod (24·prime(n))) == 1,
 * which is checked using only 64‑bit arithmetic. The algorithm runs in well under
 * 2 s for n ≤ 10 000.
 */

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0; /* offset start at 1 */

    /* Get the nth prime */
    uint32_t q = get_prime(n);          /* q fits comfortably in 32 bits */
    /* Compute modulus 24*q; fits in 64 bits for all n tested */
    uint64_t mod = 24ULL * (uint64_t)q;

    /* Search for the least prime ≥5 satisfying (p^2 mod mod) == 1 */
    uint64_t p = 5;
    while (1) {
        uint64_t t = p % mod;
        /* Compute t*t modulo mod; t < mod < 2.6M, so t*t < 7e12 fits in 64 bits */
        uint64_t t2 = (t * t) % mod;
        if (t2 == 1) {
            /* Condition satisfied: return p as seq_t */
            return (seq_t)p;
        }
        /* Move to the next prime; ensure the next_prime call receives next integer */
        p = next_prime(p + 1);
        /* Safety guard: if p overflows we stop and return 0 */
        if (p == 0) return 0;
    }
}
