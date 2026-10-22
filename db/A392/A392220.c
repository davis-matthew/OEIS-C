/*
 * A392220  A modified Sisyphus function: a(n) = concatenation of (number of
 *          digits in n) (number of odd digits in n) (number of even digits in
 *          n).
 * https://oeis.org/A392220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = conc(#digits(n), #odd_digits(n), #even_digits(n))
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

seq_t a(idx_t n)
{
    /* count digits of n */
    int d = digits_of(n);

    /* count odd digits */
    int odd = 0;
    idx_t m = n;
    if (m == 0) {
        odd = 0;            /* '0' is even */
    } else {
        while (m > 0) {
            int digit = m % 10;
            if (digit & 1)
                odd++;
            m /= 10;
        }
    }

    int even = d - odd;

    /* construct the concatenation using a larger intermediate type
       to avoid overflow for small seq_t sizes (e.g. 8‑bit)    */
    uint64_t res = 0;
    int k = digits_of((idx_t)odd);          /* digits of odd count   */
    res = (uint64_t)d * (uint64_t)pow10(k) + (uint64_t)odd;
    int l = digits_of((idx_t)even);         /* digits of even count  */
    res = res * (uint64_t)pow10(l) + (uint64_t)even;

    return (seq_t)res;
}
