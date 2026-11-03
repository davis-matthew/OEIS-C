/*
 * A396712  Expansion of e.g.f. -W_2(-x)/(1 + W_2(-x)), where W_k(x) is the
 *          k-th iterate of LambertW(x).
 * https://oeis.org/A396712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} n^(n-k) * binomial(n-1,k-1) * k^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   binomial, ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         offset: 1 (a(1)=1)                                               */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, 0 is unused */

    /* Pre‑compute powers of n: n^0 .. n^(n-1) */
    seq_t *pow_n = mem_new(seq_t, (n + 1));
    if (!pow_n) return 0;          /* allocation failure – return 0 */
    pow_n[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow_n[i] = pow_n[i-1] * (seq_t)n;   /* (n^i) */

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term = pow_n[n - k];                /* n^(n-k)             */
        term = term * binomial(n - 1, k - 1);     /* * C(n-1,k-1)        */
        term = term * ipow_u64_idx(k, k);             /* * k^k                */
        sum += term;
    }

    mem_free(pow_n);
    return sum;
}
