/*
 * A355780  E.g.f. satisfies A(x) = (1 + x)^(2 * A(x)).
 * https://oeis.org/A355780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 2^k * (k+1)^(k-1) * Stirling1(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   mul_small, pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (Stirling1: signed Stirling numbers of the first kind) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"       /* pow2(idx_t) -> seq_t */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) -> seq_t */
#include "../utils/mul_small.h" /* mul_small(seq_t x, unsigned int m) -> seq_t */

#define MAX_N   20000     /* safe upper bound for test cases   */

static seq_t  a_cache[MAX_N+1];      /* memoised a values                */
static idx_t  computed_max = 0;      /* highest n already computed      */
static seq_t  prev_row[MAX_N+1];     /* Stirling numbers for row n-1    */
static seq_t  cur_row[MAX_N+1];      /* Stirling numbers for row n      */
static seq_t  factor[MAX_N+1];       /* pre‑computed 2^k*(k+1)^(k-1)    */
static bool   initialized = false;   /* initialisation flag             */

seq_t a(idx_t n)
{
    /* ensure the global tables are set up once */
    if (!initialized) {
        a_cache[0]   = (seq_t)1;
        prev_row[0]  = (seq_t)1;          /* S(0,0)=1                         */
        factor[0]    = (seq_t)1;          /* 2^0 * 1^(−1) = 1                  */
        computed_max = 0;
        initialized  = true;
    }

    /* if n is beyond our static tables we return 0 (should not happen) */
    if (n > MAX_N) {
        return (seq_t)0;
    }

    /* extend the cached values up to n */
    if (n > computed_max) {
        /* pre‑compute any missing factor values */
        for (idx_t k = 1; k <= n; ++k) {
            if (factor[k] == (seq_t)0) {
                seq_t pow2_   = pow2(k);
                seq_t pow_base;
                if (k == 0) {                 /* never used for k>=1 */
                    pow_base = (seq_t)1;
                } else {
                    pow_base = seq_pow((seq_t)(k+1), (idx_t)(k-1));
                }
                factor[k] = pow2_ * pow_base;
            }
        }

        /* compute Stirling numbers and a(n) for rows computed_max+1 .. n */
        for (idx_t m = computed_max + 1; m <= n; ++m) {
            /* cur_row[0] = 0 for all n > 0 */
            cur_row[0] = (seq_t)0;

            /* compute signed Stirling numbers of the first kind:
               S(n,k) = S(n-1,k-1) - (n-1)*S(n-1,k)  */
            for (idx_t k = 1; k <= m; ++k) {
                seq_t mul = mul_small(prev_row[k], (unsigned int)(m-1));
                cur_row[k] = prev_row[k-1] - mul;
            }

            /* compute a(m) = Σ_{k=1..m} factor[k] * cur_row[k]  */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                sum += factor[k] * cur_row[k];
            }
            a_cache[m] = sum;

            /* prepare for next iteration: new row becomes previous row */
            for (idx_t k = 0; k <= m; ++k) {
                prev_row[k] = cur_row[k];
            }
        }

        computed_max = n;
    }

    return a_cache[n];
}
