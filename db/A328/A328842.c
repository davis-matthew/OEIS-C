/*
 * A328842  Decrement each nonzero digit by one in primorial base
 *          representation of n, then convert back to decimal.
 * https://oeis.org/A328842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - A328841(n) * * A328842 is obtained by writing n in
 *            primorial base * (base of successive primes) and subtracting 1
 *            from each * non‑zero digit. The effect is to subtract the weight
 *            of * every prime position whose digit is non‑zero
 * Range:     u8 n <= 503, u16 n <= 9362, u32 n <= 9362, u64 n <= 9362,
 *            u128 n <= 9362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* First sixteen primes: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53 */
static const uint64_t _primes[16] = {
    2ULL,  3ULL,  5ULL,  7ULL, 11ULL, 13ULL,
   17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL,
   41ULL, 43ULL, 47ULL, 53ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* a(0) = 0 */

    uint64_t tmp = n;             /* copy of n for division */
    seq_t sum = 0;                /* sum of weights to subtract */
    seq_t weight = 1;             /* primorial weight P_{i-1} */

    size_t i = 0;
    while (tmp > 0 && i < 16) {
        uint64_t p = _primes[i];
        uint64_t digit = tmp % p;  /* current primorial digit */
        if (digit != 0)
            sum += weight;        /* subtract this weight */
        tmp /= p;                 /* shift to next digit   */
        weight *= p;              /* update weight         */
        ++i;
    }

    /* For 64‑bit n the loop always terminates with tmp == 0,
       because the product of the first 16 primes already exceeds
       2^64.  No further primes are needed. */

    return (seq_t)n - sum;
}
