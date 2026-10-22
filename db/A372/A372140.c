/*
 * A372140  a(n) = Product_{k=1..n} BarnesG(k)^k.
 * https://oeis.org/A372140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} BarnesG(k)^k
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where BarnesG(k) = G(k-1) with G the Barnes G‑function.
   For integers, G(m+1)=∏_{i=1}^{m-1} i!.
   Hence BarnesG(k) = ∏_{i=1}^{k-2} i!  (empty product = 1 for k≤2). */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* Compute a(n) as defined in OEIS A372140 */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;   /* start with 1 */
    seq_t B = (seq_t)1;        /* BarnesG(1) = 1 */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* Update BarnesG(k) from BarnesG(k-1) */
        if (k >= (idx_t)2) {
            seq_t fact = factorial(k - (idx_t)2);   /* (k-2)! */
            B = seq_mul(B, fact);
        }

        /* Multiply result by B^k */
        seq_t powBk = big_pow(B, (seq_t)k);
        result = seq_mul(result, powBk);
    }

    return result;
}
