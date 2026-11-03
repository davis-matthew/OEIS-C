/*
 * A345940  Factorial of the largest prime factor of n, read modulo n: a(n) =
 *          A006530(n)! mod n.
 * https://oeis.org/A345940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A006530(n)! mod n
 * Range:     u8 n <= 288, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute p! modulo mod using 128-bit arithmetic to avoid overflow.
   The result always fits in uint64_t because mod <= p! % mod < mod. */
static seq_t factorial_mod_u64(uint64_t p, uint64_t mod) {
    uint64_t res = 1 % mod;
    for (uint64_t i = 2; i <= p; ++i) {
        res = ((unsigned __int128)res * i) % mod;
    }
    return (seq_t)res;
}

/* Largest prime factor of n (works for n up to 64-bit).
   For n <= 1, returns 1. */
static uint64_t largest_prime_factor(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t maxp = 1;

    /* Remove factor 2 */
    while ((n & 1ULL) == 0) {
        maxp = 2;
        n >>= 1;
    }

    /* Check odd factors */
    for (uint64_t i = 3; i <= n / i; i += 2) {
        while (n % i == 0) {
            maxp = i;
            n /= i;
        }
    }
    if (n > 1) maxp = n;
    return maxp;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    if (nn <= 1) return (seq_t)0;
    uint64_t p = largest_prime_factor(nn);
    return factorial_mod_u64(p, nn);
}
