/*
 * A386922  Denominators of the partial sums of 1/d(prime(k)+1), where d is
 *          the number of divisors function.
 * https://oeis.org/A386922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator (sum_{k=1..n} 1 / d(p_k + 1))
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 2028, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here p_k is the k-th prime number and d(m) = number of divisors of m. */

#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 1;                /* undefined for n = 0, return 1 */

    seq_t num = 0;               /* current numerator of the partial sum */
    seq_t den = 1;               /* current denominator of the partial sum */

    for (idx_t k = 1; k <= n; ++k) {
        uint32_t prime_val = get_prime(k);          /* p_k */
        uint64_t d_val   = divisor_count_u64((uint64_t)prime_val + 1ULL);
        seq_t   D       = (seq_t)d_val;             /* denominator of the new term */

        seq_t new_num   = num * D + den;            /* num/den + 1/D */
        seq_t new_den   = den * D;

        seq_t g          = gcd(new_num, new_den);   /* reduce the fraction */

        num = new_num / g;
        den = new_den / g;
    }

    return den;   /* a(n) is the denominator of the reduced sum */
}
