/*
 * A270392  The sum of the digits in the periodic part of the decimal
 *          expansion of 1/n.
 * https://oeis.org/A270392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   _power2, _power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula: a(n) = sum of the decimal digits that form the minimal
 *  repeating block of the decimal expansion of 1/n.
 *  The computation takes care of the leading non‑repeating part
 *  by scaling the numerator with a power of 2 or 5.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_power2.h"
#include "../utils/_power5.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, safe guard */

    /* Strip all factors 2 and 5 from n. */
    idx_t m = n;
    size_t exp2 = 0, exp5 = 0;
    while ((m & 1ULL) == 0) { m >>= 1; exp2++; }
    while (m % 5ULL == 0) { m /= 5ULL; exp5++; }

    /* If there are no remaining factors, the decimal terminates. */
    if (m == 1) return 0;

    /* Determine the scaling factor p that precedes the repeating block. */
    seq_t p = 1;
    if (exp2 > exp5) {
        p = _power5(exp2 - exp5);
    } else if (exp5 > exp2) {
        p = _power2(exp5 - exp2);
    }   /* else exp2 == exp5, p stays 1 */

    /* Start long division with numerator p and denominator m. */
    idx_t rem = (idx_t)(p % m);    /* current remainder */
    size_t size = (size_t)m;      /* maximum possible period length */

    int *first = (int *)malloc(size * sizeof(int));
    if (!first) return 0;
    for (size_t i = 0; i < size; ++i) first[i] = -1;

    unsigned char *digits = (unsigned char *)malloc(size * sizeof(unsigned char));
    if (!digits) { free(first); return 0; }

    size_t pos = 0;
    while (rem != 0 && first[(size_t)rem] == -1) {
        first[(size_t)rem] = (int)pos;
        uint64_t d = (rem * 10ULL) / m;       /* next digit in decimal */
        rem = (rem * 10ULL) % m;              /* new remainder */
        digits[pos++] = (unsigned char)d;
    }

    seq_t sum = 0;
    if (rem != 0) {                /* a repeating cycle has been detected */
        size_t start = (size_t)first[(size_t)rem];
        for (size_t i = start; i < pos; ++i)
            sum += (seq_t)digits[i];
    }

    free(first);
    free(digits);
    return sum;
}
