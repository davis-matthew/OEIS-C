/*
 * A244505  Greater of twin primes of (40n-23,40n-21).
 * https://oeis.org/A244505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the greater of the twin primes (40k-23, 40k-21) for the
 *            nth k that makes both numbers prime. a(n) = 40*k - 21 where k is
 *            the nth positive integer such that isPrime(40k-23) &&
 *            isPrime(40k-21)
 * Range:     u8 n <= 2, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined offset, return 0 for safety */

    seq_t count = 0;
    idx_t k = 1;

    while (true) {
        idx_t p1 = 40ULL * k - 23;
        idx_t p2 = 40ULL * k - 21;

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2)) {
            ++count;
            if (count == n) {
                return (seq_t)p2;  /* greater of the twin primes */
            }
        }
        ++k;
    }
}
