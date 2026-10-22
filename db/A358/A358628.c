/*
 * A358628  Square array A(i,j), i >= 0, j >= 0, read by antidiagonals: A(i,j)
 *          = Sum_{|X|=0..i} Sum_{|Y|=0..i} Product_{k=1..j} (1+X(k)+Y(k)),
 *          where X and Y are multi-indices of length j.
 * https://oeis.org/A358628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 111, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A358628  -  OEIS sequence a(n) obtained by reading
    the square array A(i,j) by antidiagonals.

    The array entry A(i,j) is given by the closed form

           A(i,j) = Σ_{k=0}^{min(i,2j)}  C(2j,k)^2 * C(3j + i - k, 3j)

    where C(n,k) is the binomial coefficient.  This follows from the
    conjectured generating function for each column j.  The mapping from
    the index n to the pair (i,j) uses the antidiagonal enumeration:

        n = T(s-1) + k          with   T(s)= (s+1)(s+2)/2
        i = k                   (row index)
        j = s - k               (column index)      */

#include <stdint.h>

/*---------------------------------------------------------------------*/
/*  Compute binomial coefficient C(n,k) for 0 ≤ k ≤ n.
    Works for any seq_t that supports * and / on integers.              */
static seq_t binom_calc(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/*---------------------------------------------------------------------*/
/*  Main sequence function.                                            */
seq_t a(idx_t n) {
    /* --- 1. Map n to (row i, column j) by antidiagonal enumeration --- */
    idx_t s = 0;          /* diagonal index (sum i+j) */
    idx_t cum = 1;        /* cumulative count up to current diagonal */
    while (n >= cum) {
        ++s;
        cum += (s + 1);   /* add next diagonal length (s+1) */
    }
    idx_t prev = cum - (s + 1);   /* cumulative count before diagonal s */
    idx_t offset = n - prev;      /* position within diagonal s */
    idx_t i = offset;             /* row index */
    idx_t j = s - offset;         /* column index */

    /* --- 2. Compute A(i,j) using the closed form sum over k ------------- */
    seq_t total = 0;
    idx_t maxk = i;
    if (2 * j < maxk) maxk = 2 * j;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binom_calc(2 * j, k);
        seq_t term = c1 * c1;                       /* C(2j,k)^2          */
        term = term * binom_calc(3 * j + i - k, 3 * j); /* *C(3j+i-k,3j)   */
        total += term;
    }
    return total;
}
