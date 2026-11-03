/*
 * A348124  Number of compositions of n where the smallest part is smaller
 *          than the number of parts.
 * https://oeis.org/A348124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - Σ_{k=1..⌊√n⌋} C(n - k^2 + k - 1, k - 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 935 (2026-11-01)
 * Helpers:   binomial, isqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/isqrt.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety for n<1, undefined otherwise */
    seq_t total = pow2(n - 1);                  /* 2^(n-1) */
    seq_t sum   = 0;
    idx_t maxk  = isqrt(n);          /* floor(sqrt(n)) */
    for (idx_t k = 1; k <= maxk; ++k) {
        /* n - k^2 + k - 1 is guaranteed non‑negative when k <= floor(sqrt(n)) */
        idx_t diff = n - k * k + k - 1;
        seq_t term = binomial(diff, k - 1);       /* C(diff, k-1) */
        sum += term;
    }
    return total - sum;                         /* a(n) = 2^(n-1) - sum */
}
