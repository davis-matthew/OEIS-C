/*
 * A386680  a(n) is the least k such that the concatenation of k, k-1, ..., 1
 *          is divisible by 2*n-1, or -1 if there is no such k.
 * https://oeis.org/A386680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = least k such that concat(k, k-1, … , 1) is divisible by
 *            (2*n-1); return -1 if no such k exists (n ≡ 3 (mod 5)). The
 *            implementation searches linearly using modular arithmetic
 * Range:     i8 n <= 33, i16 n <= 2241, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits.h"

/* fast modular exponentiation of 10^e (mod mod). */
static inline uint64_t pow10mod(uint64_t e, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t base   = 10 % mod;
    while (e) {
        if (e & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        e >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n) {
    /* The sequence is known to have no solution when n ≡ 3 (mod 5). */
    if (n % 5ULL == 3ULL)
        return (seq_t)-1;

    /* Modulus m = 2*n - 1. */
    uint64_t m = 2ULL * (uint64_t)n - 1ULL;
    if (m == 0)               /* safety for n = 0 (not in the offset) */
        return (seq_t)-1;

    uint64_t s        = 0;          /* current concatenation modulo m */
    uint64_t pow10_len = 1 % m;     /* 10^len (mod m), len = digits already used */

    for (idx_t k = 1; ; ++k) {
        unsigned int d = digits(k);                /* number of decimal digits of k */
        uint64_t pow10_d = pow10mod(d, m);         /* 10^d (mod m) */

        /* Update the concatenation modulo m: s = (k * 10^len + s) mod m. */
        uint64_t kmod = (uint64_t)k % m;
        s = ((unsigned __int128)kmod * pow10_len + s) % m;

        if (s == 0)
            return (seq_t)k;     /* found the least k */

        /* Prepare pow10_len for the next iteration: multiply by 10^d. */
        pow10_len = ((unsigned __int128)pow10_len * pow10_d) % m;
    }
}
