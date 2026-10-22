/*
 * A370792  Expansion of Product_{k>=1} (1 + 3^(k+1)*x^k) * (1 + 3^(k-1)*x^k).
 * https://oeis.org/A370792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Product_{k>=1} (1 + 3^(k+1)*x^k) *
 *            (1 + 3^(k-1)*x^k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

static seq_t *dp = NULL;          /* array of coefficients */
static idx_t  computed_up_to = (idx_t)-1; /* largest n for which dp[n] is valid */

/* Recompute all coefficients up to limit (inclusive) from scratch */
static void recompute_all(idx_t limit)
{
    seq_t *newdp = (seq_t *)malloc(sizeof(seq_t) * (limit + 1));
    if (!newdp) return;          /* out of memory – caller will see 0 */

    for (idx_t i = 0; i <= limit; ++i) newdp[i] = 0;
    newdp[0] = (seq_t)1;         /* base case */

    /* power variables:
       pow_minus1 = 3^(k-1)
       pow_plus1  = 3^(k+1)
       ab         = 3^(2k)   (product of the two k‑terms)
    */
    seq_t pow_minus1 = (seq_t)1;   /* for k = 1 : 3^0 */
    seq_t pow_plus1  = (seq_t)9;   /* for k = 1 : 3^2 */
    seq_t ab         = (seq_t)9;   /* for k = 1 : 3^2 */

    for (idx_t k = 1; k <= limit; ++k)
    {
        seq_t sum_coeff = pow_minus1 + pow_plus1; /* A_k + B_k */

        /* Update dp from high indices to low so that old values are not reused */
        for (idx_t j = limit; ; --j)
        {
            seq_t val = newdp[j];
            if (val) {
                if (j + k <= limit) {
                    newdp[j + k] += seq_mul(val, sum_coeff);
                }
                if (j + 2 * k <= limit) {
                    newdp[j + 2 * k] += seq_mul(val, ab);
                }
            }
            if (j == 0) break;
        }

        /* Prepare power variables for next k */
        pow_minus1 = mul_small(pow_minus1, 3);
        pow_plus1  = mul_small(pow_plus1, 3);
        ab         = mul_small(ab, 9);
    }

    /* Replace old array */
    if (dp) free(dp);
    dp = newdp;
    computed_up_to = limit;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (!dp || n > computed_up_to)
        recompute_all(n);

    return dp[n];
}
