/*
 * A338715  Smallest prime ending with decimal expansion of n, for n
 *          relatively prime to 10.
 * https://oeis.org/A338715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest prime p such that p ≡ m (mod 10^d), where m is
 *            the n‑th positive integer coprime to 10 (i.e. with gcd(m,10)=1)
 *            and d is the number of decimal digits of m
 * Range:     u8 n <= 8, u16 n <= 411, u32 n <= 2226, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the prime by
   iterating over numbers that end with the digit string
   of m. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Determine the n‑th integer coprime to 10.
       Coprime numbers come in the pattern 1,3,7,9 modulo 10. */
    static const idx_t residues[4] = {1,3,7,9};
    idx_t k = (n - 1) / 4;                   /* block index */
    idx_t r = residues[(n - 1) % 4];          /* residue in {1,3,7,9} */
    idx_t m = 10 * k + r;                    /* the target integer */

    /* Compute 10^digits(m).  This is the step size to preserve the suffix. */
    idx_t tmp = m;
    seq_t pow10 = 1;
    /* At least one digit; loop until tmp becomes zero. */
    do {
        pow10 *= 10;
        tmp /= 10;
    } while (tmp > 0);

    /* Search for the smallest prime ending with the decimal expansion of m. */
    seq_t candidate = (seq_t)m;
    while (!is_prime_big(candidate)) {
        candidate += pow10;
    }
    return candidate;
}
