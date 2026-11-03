/*
 * A347989  a(n) = [x^n] (2*n)! * Sum_{k=0..2*n} binomial(x+k,k).
 * https://oeis.org/A347989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (2*n)! * Sum_{k=0..2*n} binomial(x+k,k) * =
 *            coefficient of x^n in ∏_{i=2}^{2*n+1} (x + i) * = elementary
 *            symmetric sum of the integers 2,3,…,2*n+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 295 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Result array e[0..n] – e[k] = e_k( {2,…,m} ) after processing m */
    seq_t *e = mem_new(seq_t, (n + 1));
    if (!e) return 0;            /* allocation failure – should not happen */

    e[0] = 1;                    /* e_0 = 1 */
    for (idx_t k = 1; k <= n; ++k)
        e[k] = 0;                /* initialize e[1..n] */

    /* iterate over the numbers 2 … 2*n+1 */
    for (idx_t i = 2; i <= 2*n + 1; ++i) {
        seq_t mul = (seq_t)i;
        idx_t maxk = (i - 1 < n) ? (i - 1) : n;  /* maximum subset size after adding i */
        for (idx_t k = maxk; k > 0; --k) {
            /* e[k] += i * e[k-1] */
            e[k] = e[k] + mul * e[k - 1];
        }
    }

    seq_t result = e[n];
    mem_free(e);
    return result;
}
