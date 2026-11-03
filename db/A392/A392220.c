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
 * Validated: standard, pass, 71 terms through n = 9698 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* count digits of n */
    int d = num_digits_idx(n);

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
    int k = num_digits_idx((idx_t)odd);          /* digits of odd count   */
    res = (uint64_t)d * (uint64_t)pow10(k) + (uint64_t)odd;
    int l = num_digits_idx((idx_t)even);         /* digits of even count  */
    res = res * (uint64_t)pow10(l) + (uint64_t)even;

    return (seq_t)res;
}
