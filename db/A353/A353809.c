/*
 * A353809  a(n) = 1 if n is of the form 4m+1 and is not the sum of two
 *          squares, otherwise 0.
 * https://oeis.org/A353809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 1 (mod 4) and n is not expressible as a sum of
 *            two squares; otherwise 0
 * Range:     u8 n <= 7224, u16 n <= 7224, u32 n <= 7224, u64 n <= 7224,
 *            u128 n <= 7224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Not of the form 4m+1 → 0 */
    if (n % 4ULL != 1ULL)
        return 0;

    uint64_t m = (uint64_t)n;   /* work in 64‑bit arithmetic */
    uint64_t temp = m;

    /* Trial division to check the prime factor condition for sums of two squares */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            /* If a prime ≡ 3 (mod 4) appears with an odd exponent,
               the number is NOT a sum of two squares. */
            if ((p % 4 == 3) && (exp & 1))
                return 1;
        }
    }
    /* Remaining prime factor > sqrt(m) */
    if (temp > 1) {
        if ((temp % 4 == 3) && 1)   /* exponent is 1 here */
            return 1;
    }
    return 0;
}
