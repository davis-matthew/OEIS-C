/*
 * A306762  Smallest integer k such that Sum_(i=1..k) lambda(i) is divisible
 *          by n, where lambda(i) is the Carmichael lambda function.
 * https://oeis.org/A306762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   _gcd_idx, _lambda_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A306762:
   a(n) = the smallest integer k such that
          Sum_{i=1..k} λ(i)  is divisible by n
   where λ(i) is the Carmichael lambda function. */

/* Required types are provided by the build environment:
   seq_t  – unsigned integer type for sequence values
   idx_t  – unsigned 64‑bit integer type for indices   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_gcd_idx.h"
#include "../utils/_lambda_value.h"

/* ------------------------------------------------------------------ */
/* Euclidean GCD for idx_t.                                          */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n=0, but defensive */

    idx_t sum_mod = 0;       /* running sum modulo n          */
    idx_t k = 0;             /* candidate k                    */

    while (true) {
        ++k;
        idx_t lambda_k = _lambda_value(k);
        sum_mod = (sum_mod + (lambda_k % n)) % n;
        if (sum_mod == 0) break;
    }

    return (seq_t)k;
}
