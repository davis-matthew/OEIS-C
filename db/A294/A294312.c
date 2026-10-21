/*
 * A294312  Expansion of e.g.f. sec(x*exp(x)).
 * https://oeis.org/A294312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/2⌋} E_{2k} * (2k)^{n-2k} * C(n, 2k) where
 *            E_{2k} are the positive Euler (secant) numbers (E_0 = 1, E_2 =
 *            1, E_4 = 5, E_6 = 61, E_8 = 1385, …)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binom_2n_2k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/binom_2n_2k.h"

/* dynamically allocated array of Euler numbers (indexed by k, i.e. E_{2k}). */
static seq_t *euler = NULL;
static idx_t euler_cap = 0;   /* largest index already computed */

/* ensure euler[k] is known for all k <= n.  The array is grown
   with realloc (C‑style cast).  The recurrence used follows from
   the signed Euler numbers recurrence, thereby avoiding negative
   intermediate values. */
static void ensure_euler(idx_t n)
{
    if (n <= euler_cap) return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (euler == NULL)
                 ? (seq_t*)calloc(new_cap, sizeof(seq_t))
                 : (seq_t*)realloc(euler, new_cap * sizeof(seq_t));
    euler = tmp;

    /* initialize first element if not yet set */
    if (euler_cap == 0) {
        euler[0] = 1;
        euler_cap = 0;
    }

    /* compute missing Euler numbers from euler_cap+1 up to n */
    for (idx_t m = euler_cap + 1; m <= n; ++m) {
        seq_t positive_sum = 0;
        seq_t negative_sum = 0;
        seq_t binom = 1;  /* C(2m, 0) */
        for (idx_t k = 0; k < m; ++k) {
            /* term = binom(2m, 2k) * euler[k] */
            seq_t term = binom * euler[k];
            if (((m - k) & 1ULL) == 0) {
                positive_sum += term;
            } else {
                negative_sum += term;
            }
            /* update binom to C(2m, 2(k+1)) */
            if (k + 1 < m) {
                seq_t num1 = (seq_t)(2 * m - 2 * k);
                seq_t num2 = (seq_t)(2 * m - 2 * k - 1);
                seq_t den1 = (seq_t)(2 * k + 2);
                seq_t den2 = (seq_t)(2 * k + 1);
                binom = (binom * num1 * num2) / (den1 * den2);
            }
        }
        euler[m] = negative_sum - positive_sum;  /* always non‑negative */
    }
    euler_cap = n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t needed_k = n >> 1;   /* floor(n/2) */
    if (euler_cap == 0) {
        euler = (seq_t*)calloc(1, sizeof(seq_t));
        euler[0] = 1;
        euler_cap = 0;
    }
    if (needed_k > euler_cap)
        ensure_euler(needed_k);

    seq_t sum = 0;
    seq_t binom = 1;   /* C(n, 0) */
    for (idx_t k = 0; k <= needed_k; ++k) {
        seq_t power;
        if (k == 0) {
            power = (n == 0) ? 1 : 0;   /* (0)^{n} */
        } else {
            power = big_pow((seq_t)(2 * k), (seq_t)(n - 2 * k));
        }

        seq_t term = euler[k] * power * binom;
        sum += term;

        /* update binom for next k: C(n, 2(k+1)) from C(n, 2k) */
        if (k + 1 <= needed_k) {
            seq_t num1 = (seq_t)(n - 2 * k);
            seq_t num2 = (seq_t)(n - 2 * k - 1);
            seq_t den1 = (seq_t)(2 * k + 2);
            seq_t den2 = (seq_t)(2 * k + 1);
            binom = (binom * num1 * num2) / (den1 * den2);
        }
    }
    return sum;
}
