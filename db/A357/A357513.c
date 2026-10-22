/*
 * A357513  a(n) = numerator of Sum_{k = 1..n} (1/k^3) * binomial(n,k)^2 *
 *          binomial(n+k,k)^2 for n >= 1 with a(0) = 0.
 * https://oeis.org/A357513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator of Σ_{k=1..n}
 *            (1/k^3)·binomial(n,k)^2·binomial(n+k,k)^2 for n ≥ 1 with a(0) =
 *            0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    /* current rational sum  num/den in lowest terms */
    seq_t num = 0;     /* numerator   */
    seq_t den = 1;     /* denominator */

    /* binomial(n,0) and binomial(n+0,0) start as 1 */
    seq_t bin_nk   = 1; /* C(n,   k)  */
    seq_t bin_nkpk = 1; /* C(n+k, k)  */

    for (idx_t k = 1u; k <= n; ++k)
    {
        /* update C(n,k)   = C(n,k-1)·(n−k+1)/k   */
        bin_nk = bin_nk * (seq_t)(n - k + 1u) / (seq_t)k;

        /* update C(n+k,k)= C(n+k−1,k−1)·(n+k)/k */
        bin_nkpk = bin_nkpk * (seq_t)(n + k) / (seq_t)k;

        /* term numerator : (C(n,k)^2)·(C(n+k,k)^2) */
        seq_t term_num = bin_nk * bin_nk;
        term_num = term_num * bin_nkpk * bin_nkpk;

        /* term denominator : k^3 */
        seq_t kseq = (seq_t)k;
        seq_t term_den = kseq * kseq * kseq;

        /* add the term: new_num = num*term_den + term_num*den,
                         new_den = den*term_den */
        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;

        /* reduce by gcd to keep numbers small */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* the rational sum is num/den in lowest terms;
       a(n) is defined as the numerator */
    return num;
}
