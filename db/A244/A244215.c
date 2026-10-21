/*
 * A244215  a(0)=0, and for n>=1, if n is the k-th Catalan number
 *          (A000108(k)), a(n) = k, otherwise the difference of the indices of
 *          the two largest Catalan numbers whose sum is less than or equal to
 *          n: a(n) = A244160(n) - A244160(n-A081290(n)).
 * https://oeis.org/A244215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 4862, u32 n <= 4862, u64 n <= 4862,
 *            u128 n <= 4862, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244215:  a(n) = n==Catalan(k)? k : A244160(n)-A244160(n-A081290(n))
   a(0)=0, a(1)=1, a(2)=2, … */
#include "../utils/catalan.h"   /* provides seq_t catalan(idx_t k) */

/* Return the index k of the largest Catalan number <= n_val.
   For n_val < 1 the result is 0, which matches a(0)=0. */
static idx_t index_of_largest_catalan_le(seq_t n_val)
{
    if ((seq_t)n_val < (seq_t)1)
        return 0;
    idx_t k = 0;
    seq_t cat = catalan(k);
    while (cat <= n_val) {
        k++;
        cat = catalan(k);
    }
    return k - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t nseq = (seq_t)n;
    idx_t k = index_of_largest_catalan_le(nseq);
    seq_t cat = catalan(k);

    if (cat == nseq)      /* n is a Catalan number */
        return (seq_t)k;

    seq_t rem = nseq - cat;
    idx_t j = index_of_largest_catalan_le(rem);
    return (seq_t)k - (seq_t)j;
}
