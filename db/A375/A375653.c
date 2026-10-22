/*
 * A375653  Expansion of e.g.f. exp(x^2 + x * exp(x^2)).
 * https://oeis.org/A375653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A375653
    Formula: a(n) = n! * Σ_{k=0..⌊n/2⌋} (n-2k+1)^k / (k! * (n-2k)!)
*/

#include <stddef.h>                    /* for size_t if needed */
#include "../utils/big_pow.h"          /* seq_t big_pow(seq_t base, seq_t exp) */

/* ---------------------------------------------------------------------- */
/*  Compute a(n) for the OEIS sequence A375653                          */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Sum over k */
    seq_t sum = 0;

    /* maximum k is floor(n/2) */
    idx_t max_k = n >> 1;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* ---- compute the coefficient --------------------------------- */
        /*   coeff = n! / (k! * (n-2k)!)  = C(n,2k) * (2k)!/k!              */
        seq_t coeff = 1;

        /* 1. binomial coefficient C(n,2k) with multiplicative formula   */
        if (k > 0)             /* when k==0 C(n,0)=1 */
        {
            /*          i goes from 1 to 2k
                coeff = coeff * (n - 2k + i) / i
            */
            for (idx_t i = 1; i <= 2 * k; ++i)
            {
                seq_t mult = (seq_t)(n - 2 * k + i);
                coeff = (coeff * mult) / (seq_t)i;   /* exact division  */
            }

            /* 2. multiply by (2k)!/k! = product of (k+1 .. 2k)           */
            for (idx_t j = k + 1; j <= 2 * k; ++j)
            {
                coeff = coeff * (seq_t)j;
            }
        }

        /* ---- compute base power ------------------------------------- */
        seq_t base = (seq_t)(n - 2 * k + 1);
        seq_t pow  = big_pow(base, (seq_t)k);  /* (n-2k+1)^k   */

        /* ---- term contribution -------------------------------------- */
        seq_t term = coeff * pow;

        /* ---- accumulate ---------------------------------------------- */
        sum += term;
    }

    return sum;
}
