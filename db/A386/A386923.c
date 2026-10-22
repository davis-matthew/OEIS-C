/*
 * A386923  a(n) = numerator(Sum_{k=1..n} d(k)/d(k+1)), where d is the number
 *          of divisors function.
 * https://oeis.org/A386923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator( sum_{k=1..n} d(k)/d(k+1) ), where d(x) is the
 *            number of divisors of x
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1, return 0 safely */

    /* Allocate divisor counts up to n+1 (inclusive). */
    idx_t limit = n + 1;
    uint32_t *div_cnt = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!div_cnt)         /* allocation failure */
        return 0;
    for (idx_t i = 0; i <= limit; ++i)
        div_cnt[i] = 0;

    /* Classic divisor‑count sieve. */
    for (idx_t i = 1; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i)
            div_cnt[j]++;

    /* Incrementally build the rational sum.  All arithmetic is in seq_t. */
    seq_t num = 0;          /* numerator of current sum */
    seq_t den = 1;          /* denominator of current sum */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t dk  = (seq_t)div_cnt[k];
        seq_t dk1 = (seq_t)div_cnt[k + 1];

        seq_t new_num = num * dk1 + den * dk;
        seq_t new_den = den * dk1;

        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    free(div_cnt);
    return num;            /* numerator of the reduced fraction */
}
