/*
 * A281961  Decimal expansion of one chronon (in seconds).
 * https://oeis.org/A281961
 *
 * Offset:    -23    (a(-23) is the first term)
 * Keywords:  nonn, cons, more
 * Formula:   a(n) = decimal digit of the number 6.266424 × 10^(-23) (one
 *            chronon). The sequence is defined only for n ≥ -23; a(-23)=6,
 *            a(-22)=2, a(-21)=6, a(-20)=6, a(-19)=4, a(-18)=2, a(-17)=4 and
 *            a(n)=0 for all n > -17
 * Range:     u8 none, u16 none, u32 n <= -17, u64 n <= -17, u128 n <= -17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 10^e for e = 0 … 6 */
static const uint64_t POW10[7] = {1ULL,10ULL,100ULL,1000ULL,10000ULL,
                                  100000ULL,1000000ULL};

seq_t a(idx_t n)
{
    /* Out of defined range: return 0 (offset < -23 or by convention).
       For n > -17 the digits are zero because the decimal expansion
       terminates after the 7 non‑zero digits. */
    if (n < -23 || n > -17)
        return (seq_t)0;

    /* 6266424 = 6,266,424 */
    const seq_t N = 6266424ULL;

    /* e = -n-17  → 0 for n = -17, … , 6 for n = -23 */
    idx_t e = -n - 17;

    /* digit extraction: divide by 10^e and take the last decimal digit. */
    seq_t digit = (N / POW10[e]) % 10;

    return digit;
}
