/*
 * A373607  Sum of the odd-indexed digits in the primorial base representation
 *          (A049345) of n.
 * https://oeis.org/A373607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd‑indexed digits in the primorial base
 *            representation of n
 * Range:     u8 n <= 9854, u16 n <= 9854, u32 n <= 9854, u64 n <= 9854,
 *            u128 n <= 9854, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Small static list of primes sufficient for n up to at least 10 000. */
    static const idx_t primes[] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    static const size_t primes_cnt = sizeof primes / sizeof primes[0];

    idx_t m   = n;           /* number to decompose */
    idx_t prim = 1;          /* product of all preceding primes (primorial) */
    idx_t idx  = 0;          /* digit position, 0 is least significant */
    seq_t sum  = 0;           /* result accumulator */

    for (size_t i = 0; i < primes_cnt && m > 0; ++i) {
        idx_t p   = primes[i];
        idx_t quot = m / prim;            /* floor division */
        seq_t digit = (seq_t)(quot % p);  /* current digit */

        if (idx & 1)                     /* odd index → add to sum */
            sum += digit;

        if (quot == 0)                   /* representation finished */
            break;

        /* Prepare for next position:
           if prim * p would exceed m, further digits are zero. */
        if (prim > m / p)
            break;
        prim *= p;
        ++idx;
    }
    return sum;
}
