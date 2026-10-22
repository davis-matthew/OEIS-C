/*
 * A326952  a(n) = A001222(A028905(n)).
 * https://oeis.org/A326952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bigOmega( sorted_digits( prime(n) ) ) where prime(n) is
 *            the nth prime number (OEIS A000040), sorted_digits arranges the
 *            decimal digits of the prime in nondecreasing order and
 *            interprets them as a decimal integer. bigOmega is the total
 *            number of prime factors counting multiplicity (OEIS A00...
 * Range:     u8 n <= 54, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   count_prime_factors, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* the sequence is offset 1; values for n<1 are unused */
    if (n == 0) return 0;

    /* step 1: obtain the n-th prime (fits in uint32_t for n<=10000) */
    uint32_t p = get_prime(n);          /* get_prime is provided */

    /* step 2: collect digit frequencies of p */
    unsigned int freq[10] = {0};
    uint32_t tmp = p;
    if (tmp == 0) freq[0] = 1;          /* only for safety */
    while (tmp > 0) {
        freq[tmp % 10]++;
        tmp /= 10;
    }

    /* step 3: build the sorted‑digit number */
    seq_t m = 0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned int k = 0; k < freq[d]; ++k) {
            m = m * 10 + d;
        }
    }

    /* step 4: count prime factor multiplicities of m */
    seq_t result = count_prime_factors(m);   /* count_prime_factors is provided */

    return result;
}
