/*
 * A384117  Array read by antidiagonals: T(n,m) is the number of minimum total
 *          dominating sets in the n X m rook graph K_n X K_m.
 * https://oeis.org/A384117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of minimum total dominating sets in the n×m rook
 *            graph, read by antidiagonals: T(n,m) counts such sets in K_n ×
 *            K_m. See the OEIS entry for the definition
 * Range:     u8 n <= 59, u16 n <= 143, u32 n <= 311, u64 n <= 758,
 *            u128 n <= 1325, big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"
#include <stdbool.h>
#include <string.h>

/* Helper: factorial for small integers (uses the provided factorial). */
static inline seq_t factu(idx_t k)
{
    return factorial(k);
}

/* Helper: compute T(n,m).  n,m are 0‑based indices. */
static seq_t total_dom_sets(idx_t n, idx_t m)
{
    /* Trivial cases. */
    if (n == 0 || m == 0)
        return (seq_t)1;
    if (n == 1 && m == 1)
        return (seq_t)0;
    if (m == 1)          /* minimal total dominating sets in K_n (n≥2): C(n,2) */
        return binomial(n, 2);
    if (n == 1)          /* symmetry */
        return binomial(m, 2);

    /* Ensure n ≥ m for formula usage. */
    if (m > n) {
        idx_t tmp = n; n = m; m = tmp;
    }

    /* Equal case: n == m ≥ 2.  Count solutions that are one per column
       or one per row.  Inclusion–exclusion gives the formula below. */
    if (n == m) {
        seq_t nfact = factu(n);
        seq_t sum   = 0;
        for (idx_t t = 0; t <= n; ++t) {
            seq_t comb   = binomial(n, t);
            seq_t nfdiv  = nfact / factu(n - t);   /* n! / (n‑t)! */
            seq_t pw     = big_pow((seq_t)(n - t), (seq_t)(n - t));
            seq_t term   = comb * nfdiv * pw;
            if (t & 1)
                sum -= term;
            else
                sum += term;
        }
        return sum * (seq_t)2;
    }

    /* General case: 1 < m < n.  Use the explicit summation. */
    seq_t result = 0;
    seq_t fact_i = 1;          /* i! for i = 0,1,2,... */

    for (idx_t i = 0; i <= m; ++i) {
        if (i > 0)
            fact_i *= (seq_t)i;        /* update i! */

        seq_t tmp = binomial(m, i);
        tmp      *= binomial(n, i);
        tmp      *= fact_i;
        tmp      *= big_pow((seq_t)(n - i), (seq_t)(m - i));

        if (i & 1)
            result -= tmp;
        else
            result += tmp;
    }
    return result;
}

/* Public entry: a(n) read from the antidiagonal of the table. */
seq_t a(idx_t n)
{
    /* Locate the diagonal s such that n is inside it. */
    idx_t s = 0;
    while (n >= (s + 1) * (s + 2) / 2)
        ++s;

    idx_t prev = s * (s + 1) / 2;   /* first index in diagonal s */
    idx_t i    = n - prev;          /* row index within the diagonal */
    idx_t j    = s - i;             /* column index */

    return total_dom_sets(i, j);
}
