/*
 * A390358  a(n) = Sum_{k=0..n} binomial(3*n+2*k,n-k).
 * https://oeis.org/A390358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n+2*k, n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iterating the terms of the sum with a multiplicative
   recurrence that keeps intermediate values small. */

#include <stddef.h>
#include "../utils/gcd.h"

static inline void reduce_factor_with_den(idx_t *factor, idx_t den[3])
{
    for (size_t i = 0; i < 3; ++i) {
        if (den[i] <= 1) continue;
        seq_t g = gcd((seq_t)(*factor), (seq_t)den[i]);
        if (g > 1) {
            *factor /= (idx_t)g;
            den[i] /= (idx_t)g;
        }
    }
}

static inline void reduce_val_with_den(seq_t *val, idx_t den[3])
{
    for (size_t i = 0; i < 3; ++i) {
        if (den[i] <= 1) continue;
        seq_t g = gcd(*val, (seq_t)den[i]);
        if (g > 1) {
            *val /= g;
            den[i] /= (idx_t)g;
        }
    }
}

static inline seq_t mul_reduce(seq_t val, idx_t factor, idx_t den[3])
{
    /* Cancel common factors between factor and the denominators first */
    reduce_factor_with_den(&factor, den);
    /* Multiply the remaining factor into val (now safe because factor is
       small or 1). */
    val *= (seq_t)factor;
    /* Reduce the result with any remaining denominators. */
    reduce_val_with_den(&val, den);
    return val;
}

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)1;       /* term for m = 0 */
    seq_t term = (seq_t)1;      /* current binomial value C(5n-2m, m) */

    for (idx_t m = 0; m < n; ++m) {
        /* Prepare numerators and denominators for the recurrence
           T(m+1) = T(m) * (5n-3m)*(5n-3m-1)*(5n-3m-2) /
                         ((m+1)*(5n-2m)*(5n-2m-1)) */
        idx_t num1 = 5 * n - 3 * m;
        idx_t num2 = 5 * n - 3 * m - 1;
        idx_t num3 = 5 * n - 3 * m - 2;

        idx_t den[3];
        den[0] = m + 1;              /* (m+1)    */
        den[1] = 5 * n - 2 * m;      /* (5n-2m)  */
        den[2] = den[1] - 1;         /* (5n-2m-1)*/

        /* Apply the three numerator factors sequentially,
           reducing with denominators to keep the product small. */
        term = mul_reduce(term, num1, den);
        term = mul_reduce(term, num2, den);
        term = mul_reduce(term, num3, den);

        /* Any remaining non‑cancelled denominators must divide the
           current term exactly. */
        for (size_t i = 0; i < 3; ++i) {
            if (den[i] > 1) {
                term /= (seq_t)den[i];
            }
        }

        sum += term;
    }

    return sum;
}
