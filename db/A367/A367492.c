/*
 * A367492  a(n) = Product_{k=0..n} (k+1)!^k.
 * https://oeis.org/A367492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} ((k+1)!)^k
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t res   = (seq_t)1;      /* product accumulator */
    seq_t fact  = (seq_t)1;      /* running factorial of (k+1)! */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update factorial: fact = (k+1)! */
        fact = seq_mul(fact, (seq_t)(k + 1));

        /* Compute ((k+1)!)^k */
        seq_t pw = ipow_idx(fact, k);

        /* Multiply into product */
        res = seq_mul(res, pw);
    }

    return res;
}
