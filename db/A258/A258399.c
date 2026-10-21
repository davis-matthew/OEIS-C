/*
 * A258399  Number of 4n-length strings of balanced parentheses of exactly n
 *          different types that are introduced in ascending order.
 * https://oeis.org/A258399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(2n) * Stirling2(2n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, binom, catalan, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"          /* binom */
#include "../utils/factorial.h"       /* factorial */
#include "../utils/big_pow.h"         /* big_pow */
#include "../utils/catalan.h"         /* catalan */

/* Stirling number of the second kind S(m, k) =
   (1/k!) * Σ_{i=0}^{k} (-1)^i * C(k, i) * (k - i)^m   */
static seq_t stirling_sum(idx_t m, idx_t k)
{
    if (k == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    seq_t factk = factorial(k);

    for (idx_t i = 0; i <= k; ++i)
    {
        seq_t bin = binom(k, i);                    /* C(k, i) */
        seq_t base = (seq_t)(k - i);                /* (k - i)   */
        seq_t pow  = big_pow(base, (seq_t)m);        /* (k - i)^m */

        seq_t term = bin * pow;                     /* C(k,i)*(k-i)^m */

        if (i & 1U)
            sum -= term;        /* subtract odd i terms */
        else
            sum += term;        /* add   even i terms    */
    }

    return sum / factk;                         /* exact division */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t cat   = catalan(2 * n);                /* Catalan(2n)   */
    seq_t stir  = stirling_sum(2 * n, n);        /* S(2n, n)      */

    return cat * stir;                           /* a(n) value    */
}
