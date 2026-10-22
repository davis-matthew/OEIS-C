/*
 * A371229  E.g.f. satisfies A(x) = 1 - x*A(x)^2*log(1 - x*A(x)^2).
 * https://oeis.org/A371229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (2*n)! * Sum_{k=0..floor(n/2)}
 *            |Stirling1(n-k,k)|/((n-k)! * (2*n-k+1)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   compute_signless_stirling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_signless_stirling.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    if (n == 1) return (seq_t)0;          /* a(1) = 0 */

    idx_t half = n / 2;
    /* allocate array for |S(n-k,k)| values */
    seq_t *s_vals = (seq_t *)calloc((size_t)(half + 1), sizeof(seq_t));
    if (!s_vals) return (seq_t)0;          /* allocation failure */

    compute_signless_stirling(n, half, s_vals);

    seq_t result = (seq_t)0;

    for (idx_t k = 1; k <= half; ++k) {
        if (s_vals[k] == (seq_t)0) continue;  /* skip zero terms */

        /* P = n * (n-1) * ... * (n-k+1)  = n! / (n-k)! */
        seq_t P = (seq_t)1;
        for (idx_t j = 0; j < k; ++j) {
            P *= (seq_t)(n - j);
        }

        /* Q = (2n) * (2n-1) * ... * (2n-k+2)
         *      = (2n)! / (2n-k+1)!    (k-1 terms; Q = 1 when k=1)
         */
        seq_t Q = (seq_t)1;
        for (idx_t i = 0; i < k - 1; ++i) {
            Q *= (seq_t)(2 * n - i);
        }

        result += s_vals[k] * P * Q;
    }

    free(s_vals);
    return result;
}
