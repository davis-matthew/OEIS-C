/*
 * A385986  a(1) = 2, and for any n > 1, a(n) is the largest k < n such that
 *          a(1) + ... + a(k) is prime.
 * https://oeis.org/A385986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385986: a(1)=2; for n>1, a(n) = (isPrime(Σ_{i=1}^{n-1} a(i)) ? n-1 : a(n-1)),
   where Σ denotes the cumulative sum up to that index. */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Handle the first index explicitly */
    if (n == 0) return 0;           /* undefined, but guard anyway */
    seq_t a_curr = 2;
    seq_t sum     = 2;

    if (n == 1) return a_curr;

    for (idx_t i = 2; ; ++i) {
        /* Check primality of the current cumulative sum */
        if (isPrimeGeneric(sum))
            a_curr = (seq_t)(i - 1);    /* new value if sum was prime */
        /* Update the cumulative sum with the chosen a(i) */
        sum += a_curr;
        /* Stop when we have produced a(n) */
        if (i == n) break;
    }

    return a_curr;
}
