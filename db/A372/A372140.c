/*
 * A372140  a(n) = Product_{k=1..n} BarnesG(k)^k.
 * https://oeis.org/A372140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} BarnesG(k)^k
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where BarnesG(k) = G(k-1) with G the Barnes G‑function.
   For integers, G(m+1)=∏_{i=1}^{m-1} i!.
   Hence BarnesG(k) = ∏_{i=1}^{k-2} i!  (empty product = 1 for k≤2). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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
        seq_t powBk = ipow(B, (seq_t)k);
        result = seq_mul(result, powBk);
    }

    return result;
}
