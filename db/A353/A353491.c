/*
 * A353491  a(1) = 1, and for n > 1, a(n) = 1 if the largest proper divisor of
 *          n is of the form 4k+1, otherwise 0.
 * https://oeis.org/A353491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the largest proper divisor of n is congruent to 1
 *            mod 4, otherwise 0; a(1) = 1
 * Range:     u8 n <= 9783, u16 n <= 9783, u32 n <= 9783, u64 n <= 9783,
 *            u128 n <= 9783, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: return the smallest prime factor of n (n >= 2).
   If n is prime, returns n itself. */
static uint64_t _smallest_prime_factor_uint64(uint64_t n) {
    if (n < 2) return n;              /* undefined for 0,1 but handled elsewhere */
    if (n % 2 == 0) return 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0)
            return i;
    }
    return n;                          /* n is prime */
}

/* Public function matching the build's types. */
seq_t a(idx_t n) {
    if (n <= 1)          /* a(1) = 1; a(0) behaves as 1 to keep the function safe */
        return (seq_t)1;

    /* Compute largest proper divisor: n / smallest prime factor */
    uint64_t spf = _smallest_prime_factor_uint64((uint64_t)n);
    uint64_t largest_proper = (uint64_t)n / spf;

    /* Return 1 if largest proper divisor ≡ 1 (mod 4), else 0 */
    return (seq_t)((largest_proper % 4U) == 1U ? 1 : 0);
}
