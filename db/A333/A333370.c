/*
 * A333370  Convolution of primorial numbers (A002110) with themselves.
 * https://oeis.org/A333370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} prime(k)# * prime(n-k)#
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Allocate array to hold primorials up to index n */
    seq_t *primorial = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!primorial) return 0;  /* allocation failure: return 0 */

    primorial[0] = 1;  /* 0# = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = (seq_t)get_prime(i);          /* i‑th prime (1‑based) */
        primorial[i] = primorial[i - 1] * p;    /* next primorial */
    }

    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        result += primorial[k] * primorial[n - k];
    }

    free(primorial);
    return result;
}
