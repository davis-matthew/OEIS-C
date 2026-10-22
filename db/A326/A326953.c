/*
 * A326953  a(n) = A001222(A028906(n)).
 * https://oeis.org/A326953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Ω( reverse_sorted_digits( prime(n) ) ) where
 *            reverse_sorted_digits gets the digits of the nth prime, sorts
 *            them descending and concatenates them into a number m; Ω(m) is
 *            the total number of prime factors (with multiplicity) of m
 * Range:     u8 n <= 9592, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   bigomega, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Prime number of index n */
    uint32_t p = get_prime(n);

    /* Frequency of each decimal digit in p */
    int freq[10] = {0};
    uint32_t t = p;
    if (t == 0) {          /* safety, never happens for primes > 1 */
        freq[0] = 1;
    } else {
        while (t > 0) {
            int d = t % 10;
            freq[d]++;
            t /= 10;
        }
    }

    /* Build the number with digits sorted in descending order */
    uint64_t m = 0;
    for (int d = 9; d >= 0; --d)
        for (int k = 0; k < freq[d]; ++k)
            m = m * 10 + (uint64_t)d;

    /* Return the number of prime factors of m, counted with multiplicity */
    return bigomega((idx_t)m);
}
