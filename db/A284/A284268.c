/*
 * A284268  Sum of coefficients > 1 in the Stern polynomial B(2n+1,x): a(n) =
 *          A275812(A277324(n)).
 * https://oeis.org/A284268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of coefficients > 1 in Stern polynomial B(2n+1, x)
 * Range:     u8 n <= 2344, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 114 terms through n = 8148 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Recursively build the coefficient vector of B(m, x) and return a pointer
   to the array and its length via the out-parameter `len`.  The caller
   is responsible for freeing the returned array. */
static seq_t *polynomial(idx_t m, size_t *len)
{
    if (m == 1) {
        *len = 1;
        seq_t *p = mem_new(seq_t, 1);
        p[0] = 1;
        return p;
    }

    if ((m & 1) == 0) {                /* even: B(2k) = x * B(k) */
        size_t l1;
        seq_t *c1 = polynomial(m >> 1, &l1);

        size_t l = l1 + 1;
        seq_t *res = mem_new(seq_t, l);
        res[0] = 0;
        for (size_t i = 0; i < l1; ++i)
            res[i + 1] = c1[i];

        mem_free(c1);
        *len = l;
        return res;
    } else {                            /* odd:  B(2k+1) = B(k) + B(k+1) */
        size_t l1, l2;
        seq_t *c1 = polynomial(m >> 1, &l1);           /* floor((m-1)/2)   */
        seq_t *c2 = polynomial((m >> 1) + 1, &l2);     /* ceil((m-1)/2)+1  */

        size_t l = (l1 > l2) ? l1 : l2;
        seq_t *res = mem_new(seq_t, l);
        for (size_t i = 0; i < l; ++i) {
            seq_t a = (i < l1) ? c1[i] : 0;
            seq_t b = (i < l2) ? c2[i] : 0;
            res[i] = a + b;
        }

        mem_free(c1);
        mem_free(c2);
        *len = l;
        return res;
    }
}

/* Primary function computing a(n). */
seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;                /* odd index of the Stern polynomial */
    size_t len;
    seq_t *coeffs = polynomial(m, &len);

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        if (coeffs[i] > 1)
            sum += coeffs[i];
    }

    mem_free(coeffs);
    return sum;
}
