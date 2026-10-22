/*
 * A345940  Factorial of the largest prime factor of n, read modulo n: a(n) =
 *          A006530(n)! mod n.
 * https://oeis.org/A345940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A006530(n)! mod n
 * Range:     u8 n <= 288, u16 n <= 9978, u32 n <= 9978, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   factorial_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial_mod_u64.h"

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
