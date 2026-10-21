/*
 * A276093  The least significant nonzero digit in primorial base is replaced
 *          with zero: a(n) = n - A276094(n), a(0) = 0.
 * https://oeis.org/A276093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - (least significant nonzero digit of n in primorial
 *            base) * × (product of all smaller primes). This finds that digit
 *            by * repeatedly dividing by successive primes
 * Range:     u8 n <= 255, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* first 25 primes – more than enough for any 64‑bit n */
static const uint64_t primorial_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL,
    83ULL, 89ULL, 97ULL
};

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 by definition */
        return 0;

    seq_t temp = (seq_t)n;   /* work on a copy of n */
    seq_t prod = 1;          /* product of primes processed so far */
    size_t i;
    for (i = 0; i < sizeof(primorial_primes)/sizeof(primorial_primes[0]); ++i) {
        seq_t prime = (seq_t)primorial_primes[i];
        seq_t digit = temp % prime;
        temp    /= prime;          /* remainder for next higher place */

        if (digit != 0) {          /* least significant non‑zero digit found */
            seq_t sub = digit * prod;   /* value to subtract from n */
            return (seq_t)n - sub;
        }

        prod *= prime;             /* move to next place value */
    }

    /* should never reach here for a valid 64‑bit input; safety return */
    return 0;
}
