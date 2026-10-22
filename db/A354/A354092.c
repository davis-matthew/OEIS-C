/*
 * A354092  Fully multiplicative prime shift where the primes of the form 3k+2
 *          are replaced by the previous such prime (with 2 -> 1), and primes
 *          of the form 3k and 3k+1 stay as they are.
 * https://oeis.org/A354092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fully multiplicative prime‑shift sequence A354092.
   For a prime p:
     - p ≡ 2 (mod 3) : a(p) = previous prime of type ≡2 (mod 3)
                         (with 2 → 1)
     - p ≡ 0 or 1 (mod 3) : a(p) = p
   For n>1, a(n) = Π a(p)^e  over prime factorisation n=∏ p^e.
   The implementation factors n by trial division and uses seq_t
   arithmetic (seq_mul, seq_pow) supplied by the build system. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Test whether x is prime (trial division, fast enough for n ≤ 100‑000). */
static bool is_prime_uint64(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2ULL;
    if (x % 3 == 0) return x == 3ULL;
    for (uint64_t d = 5ULL; d * d <= x; d += 6ULL) {
        if (x % d == 0 || x % (d + 2ULL) == 0) return false;
    }
    return true;
}

/* Find the previous prime < p that is ≡ 2 (mod 3).  Assumes p > 2 and p % 3 == 2. */
static uint64_t prev_prime_of_type(uint64_t p) {
    uint64_t q = p - 1ULL;
    for (;;) {
        if (q <= 2ULL) return 2ULL;     /* should not happen for valid inputs */
        if (q % 3 == 2ULL && is_prime_uint64(q)) return q;
        --q;
    }
}

/* Map a prime to its value in the sequence. */
static seq_t prime_map(uint64_t p) {
    if (p == 2ULL) return (seq_t)1;
    if (p % 3 == 0ULL || p % 3 == 1ULL) return (seq_t)p;
    uint64_t prev = prev_prime_of_type(p);
    return (seq_t)prev;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 1ULL) return (seq_t)1;

    uint64_t m = n;
    seq_t result = (seq_t)1;

    /* Trial division factorisation */
    for (uint64_t p = 2ULL; p * p <= m; ++p) {
        if (m % p == 0ULL) {
            uint64_t exp = 0ULL;
            while (m % p == 0ULL) {
                m /= p;
                ++exp;
            }
            seq_t base = prime_map(p);
            seq_t power = seq_pow(base, (idx_t)exp);
            result = seq_mul(result, power);
        }
    }

    /* Remaining prime factor (if any) */
    if (m > 1ULL) {
        seq_t base = prime_map(m);
        result = seq_mul(result, base);   /* exponent is 1 */
    }

    return result;
}
