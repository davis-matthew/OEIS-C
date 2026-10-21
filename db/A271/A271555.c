/*
 * A271555  a(n) = G_n(8), where G is the Goodstein function defined in
 *          A266201.
 * https://oeis.org/A271555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A271555:  a(n) = G_n(8)  (Goodstein function with base 8)
    The implementation follows the definition:
        a_0 = 8
        a_k = B_{k+2}(a_{k-1}) – 1
    where B_{b}(x) is the value obtained by writing x in hereditary
    base b and then replacing each occurrence of the digit b by
    b+1.  The helper function `transform_rec` computes B_{b+1}(x)
    recursively.  No pre‑computed tables are used – the result can
    be produced for any n in a practical amount of time.  */
#include <stdint.h>
#include "../utils/big_pow.h"

/* Recursive helper: compute B_{base+1}(x) for a given base (>=2).
   The function evaluates the hereditary base representation of
   `x` in base `base`, replaces every occurrence of the digit
   `base` with `base+1` (both as bases and as exponents) and
   returns the resulting numeric value. */
static seq_t
transform_rec(seq_t x, unsigned base)
{
    if (x == (seq_t)0)
        return (seq_t)0;

    seq_t result = (seq_t)0;
    const seq_t base_plus1 = (seq_t)(base + 1);
    seq_t tmp = x;
    unsigned exp_index = 0;          /* digit position (exponent)        */

    while (tmp > (seq_t)0) {
        const seq_t coeff = tmp % (seq_t)base;
        if (coeff != (seq_t)0) {
            /* compute transformed exponent: B_{base+1}(exp_index) */
            const seq_t exp_val = transform_rec((seq_t)exp_index, base);

            /* term = coeff * (base+1)^{exp_val} */
            seq_t term = big_pow(base_plus1, exp_val);
            term *= coeff;
            result += term;
        }
        tmp /= (seq_t)base;
        ++exp_index;
    }
    return result;
}

/* Main sequence function.
   a(n) = G_n(8) with G_0(8) = 8.  The loop performs `n` iterations,
   each time applying the transformation for the next base
   (first base = 2).  After the transformation the value is
   decremented by 1 as defined in the Goodstein function. */
seq_t
a(idx_t n)
{
    seq_t val = (seq_t)8;          /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        const unsigned base = (unsigned)(i + 2);   /* bases 2,3,4,... */
        val = transform_rec(val, base) - (seq_t)1;
    }
    return val;
}
