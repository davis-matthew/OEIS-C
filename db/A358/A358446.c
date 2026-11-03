/*
 * A358446  a(n) = n! * Sum_{k=0..floor(n/2)} 1/binomial(n-k, k).
 * https://oeis.org/A358446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} 1/binomial(n-k, k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 438 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate space for factorials up to n */
    seq_t *fac = mem_new(seq_t, (size_t)(n + 1));
    if (!fac)                 /* allocation failure → 0 (unlikely for reasonable n) */
        return (seq_t)0;

    fac[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fac[i] = fac[i - 1] * (seq_t)i;          /* fac[i] = i! */

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t denom   = binomial(n - k, k);      /* binomial(n-k,k) */
        seq_t term    = fac[n] / denom;          /* exact division */
        sum += term;
    }

    mem_free(fac);
    return sum;
}
