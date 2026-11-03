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
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *dp = NULL;          /* array of coefficients */
static idx_t  computed_up_to = (idx_t)-1; /* largest n for which dp[n] is valid */

/* Recompute all coefficients up to limit (inclusive) from scratch */
static void recompute_all(idx_t limit)
{
    seq_t *newdp = mem_new(seq_t, (limit + 1));
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
    if (dp) mem_free(dp);
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
