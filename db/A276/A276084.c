/*
 * A276084  a(n) = Number of trailing zeros in primorial base representation
 *          of n (A049345); largest k such that A002110(k) divides n.
 * https://oeis.org/A276084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1283, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A276084:
   a(n) = largest k such that the primorial of k divides n
   (i.e. the number of trailing zeros in the primorial‑base representation of n).
   The routine tests successive primorials until one fails to divide n. */

#include <stddef.h>
#include <stdint.h>

static const uint64_t _primes[15] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
    17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL,
    41ULL, 43ULL, 47ULL
};

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, but guard just in case */

    seq_t sn = (seq_t)n;           /* convert index to seq_t type */
    seq_t prod = 1;                /* product of primes seen so far */
    seq_t result = 0;              /* number of trailing zeros */

    for (size_t i = 0; i < sizeof(_primes)/sizeof(_primes[0]); ++i) {
        seq_t p = (seq_t)_primes[i];
        prod *= p;

        if (prod > sn) break;      /* cannot divide if product exceeds n */

        if (sn % prod == 0) {
            ++result;              /* this primorial divides n */
        } else {
            break;                 /* any larger primorial will fail as well */
        }
    }
    return result;
}
