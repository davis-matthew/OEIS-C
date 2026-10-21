/*
 * A299700  Squarefree part of 1!*2!*3!*...*n!: The product of factorials one
 *          through n divided by its largest square divisor.
 * https://oeis.org/A299700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007913(A000178(n))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 108,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the squarefree part of the product 1!*2!*...*n! */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but harmless */

    /*------------------------------------------------------------------
       Persistent SPF table for all calls.
       It is rebuilt only when the requested limit increases.
    ------------------------------------------------------------------*/
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;

    if (n > spf_limit) {
        if (spf) free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    /*------------------------------------------------------------------
       Parity array for primes ≤ n.
       parity[p] == true  ⇔  exponent of p in the superfactorial has odd parity.
    ------------------------------------------------------------------*/
    bool *parity = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!parity) return 0;  /* allocation failure – unlikely for n ≤ 10000 */

    for (idx_t j = 1; j <= n; ++j) {
        idx_t weight = n - j + 1;         /* how many factorials contain j */
        if ((weight & 1) == 0) continue; /* even weight gives no parity change */

        idx_t tmp = j;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (cnt & 1) parity[p] = !parity[p];
        }
    }

    /*------------------------------------------------------------------
       Build the squarefree product from the parity array.
    ------------------------------------------------------------------*/
    seq_t result = 1;
    for (idx_t p = 2; p <= n; ++p) {
        if (parity[p]) {
            result = result * (seq_t)p;
        }
    }

    free(parity);
    return result;
}
