/*
 * A271978  G_7(n), where G is the Goodstein function defined in A266201.
 * https://oeis.org/A271978
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  G_7(n) – Goodstein function iterated 7 times
 *  Definition (OEIS A271978, via A266201):
 *  Let G_0(n)=n.  For k≥1,
 *        G_k(n) = B_{k+1}(G_{k-1}(n)) – 1
 *  where B_b(x) is the result of interpreting the hereditary base‑b
 *  representation of x in base (b+1).  The function a(n)=G_7(n) is
 *  defined for all n≥3; a(n)=0 for n<3.
 */

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp) */
#include <stdbool.h>            /* bool – not strictly needed, but harmless */

/*  Recursive helper that computes B_b(x) as described above.
    Parameters:
        base – the base (b) used for the hereditary representation.
        x    – the number to transform.
    Returns the value of B_b(x).  The recursion handles all
    required conversions for exponents and coefficients. */
static seq_t B_rec(idx_t base, seq_t x)
{
    if (x < (seq_t)base)
        return x;                       /* no base symbols to replace */

    /* Determine largest exponent e such that base^e ≤ x. */
    idx_t      e   = 0;
    seq_t      tmp = x;
    while (tmp >= (seq_t)base) {
        tmp /= (seq_t)base;
        ++e;
    }

    seq_t coeff   = tmp;                                 /* floor(x / base^e)   */
    seq_t pow_base = big_pow((seq_t)base, (seq_t)e);        /* base^e              */
    seq_t rest    = x - coeff * pow_base;                 /* remaining part      */

    seq_t e_val   = B_rec(base, (seq_t)e);                 /* interpret exponent  */
    seq_t term_val = big_pow((seq_t)(base + 1), e_val);    /* base+1 ^ e_val      */

    return coeff * term_val + B_rec(base, rest);           /* sum of the parts   */
}

/*  Main sequence function.  It follows the Goodstein recurrence
    for 7 iterations.  If the intermediate value reaches 0,
    any further iterations would stay 0, so we terminate early. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* offset: a(3) is first defined term */

    seq_t g = (seq_t)n;
    for (idx_t i = 1; i <= 7; ++i) {
        g = B_rec(i + 1, g);            /* B_{i+1} of current value   */
        if (g == 0)            /* any further -1 would wrap; terminate early */
            return 0;
        --g;                     /* subtract 1 as per definition */
    }
    return g;
}
