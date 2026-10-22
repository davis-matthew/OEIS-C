/*
 * A343525  If n = Product (p_j^k_j) then a(n) = Product (2*p_j^k_j + 1), with
 *          a(1) = 1.
 * https://oeis.org/A343525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, mult
 * Formula:   a(n) = Π_{p^k‖n} (2·p^k + 1), a(1) = 1
 * Range:     u8 n <= 29, u16 n <= 2309, u32 n <= 9874, u64 n <= 9874,
 *            u128 n <= 9874, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the product of (2 * p^k + 1) over all prime powers in n */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t  rem    = n;

    /* Handle factor 2 separately */
    if ((rem & 1ULL) == 0) {
        seq_t p_pow = (seq_t)1;
        while ((rem & 1ULL) == 0) {
            p_pow *= (seq_t)2;
            rem >>= 1;
        }
        result *= ((seq_t)2 * p_pow + (seq_t)1);
    }

    /* Check odd factors up to sqrt(rem) */
    for (idx_t i = 3; i <= rem / i; i += 2) {
        if (rem % i == 0) {
            seq_t p_pow = (seq_t)1;
            while (rem % i == 0) {
                p_pow *= (seq_t)i;
                rem /= i;
            }
            result *= ((seq_t)2 * p_pow + (seq_t)1);
        }
    }

    /* Remaining prime factor, if any */
    if (rem > 1) {
        seq_t p_pow = (seq_t)rem;
        result *= ((seq_t)2 * p_pow + (seq_t)1);
    }

    return result;
}
