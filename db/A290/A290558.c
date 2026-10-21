/*
 * A290558  Coefficients in 7-adic expansion of sqrt(2).
 * https://oeis.org/A290558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th digit in the 7‑adic expansion of √2 (a(0)=4,
 *            for n>0 a(n)=(-c_n) mod 7 where c_n is the coefficient of 7^n in
 *            (x_n^2-2) with x_n the integer formed by the first n digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm is O(n^2) for n up to 10000, 2.0 sec is plenty. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* offset 0 */
    if (n == 0) return (seq_t)4;

    size_t N = (size_t)n;                     /* number of digits needed */
    unsigned char *d = (unsigned char *)malloc((N + 1) * sizeof(unsigned char));
    if (!d) return (seq_t)0;                  /* memory failure */
    int64_t *conv = (int64_t *)malloc(N * sizeof(int64_t));
    if (!conv) { free(d); return (seq_t)0; }

    /* first digit */
    d[0] = 4;
    conv[0] = 16;          /* 4*4 */

    for (size_t k = 1; k <= N; ++k)            /* compute digit a_k */
    {
        int64_t carry = 0;
        /* t = 0 */
        int64_t val = conv[0] - 2 + carry;
        carry = val / 7;

        /* t = 1 .. k-1 */
        for (size_t t = 1; t < k; ++t) {
            val = conv[t] + carry;
            carry = val / 7;
        }

        /* compute b_k = sum_{i=1}^{k-1} d[i]*d[k-i] */
        int64_t bk = 0;
        for (size_t i = 1; i <= k - 1; ++i) {
            bk += (int64_t)d[i] * d[k - i];
        }

        val = bk + carry;
        int ck = (int)(val % 7);              /* coefficient of 7^k */

        int ak = (7 - ck) % 7;                /* digit a_k */
        d[k] = (unsigned char)ak;

        /* compute conv[k] = 2*d[0]*d[k] + bk, used in later steps */
        if (k < N) {
            int64_t convk = 2 * (int64_t)d[0] * d[k] + bk;
            conv[k] = convk;
        }
    }

    seq_t result = (seq_t)d[N];
    free(d);
    free(conv);
    return result;
}
