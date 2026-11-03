/*
 * A300519  Convolution of n! and n^n.
 * https://oeis.org/A300519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^k * (n-k)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Pre‑compute factorials up to n */
    seq_t *fact = mem_new(seq_t, (n + 1));
    if (!fact)
        return (seq_t)0;               /* out of memory: safe fallback */

    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Compute the convolution sum */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t pk;                       /* k^k */
        if (k == 0)
            pk = (seq_t)1;              /* 0^0 is defined as 1 */
        else
            pk = ipow((seq_t)k, (seq_t)k);

        sum += pk * fact[n - k];
    }

    mem_free(fact);
    return sum;
}
