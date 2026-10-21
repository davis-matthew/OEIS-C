/*
 * A277950  Triangle read by rows, in which row n gives coefficients in
 *          expansion of ((x^n - 1)/(x - 1))^5.
 * https://oeis.org/A277950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = Σ_{i=0}^{⌊k / r⌋} (-1)^i * C(5,i) * C(k+4-i*r,4) where r
 *            is the unique row with Σ_{j=1}^{r-1}(5j-4) < n ≤
 *            Σ_{j=1}^{r}(5j-4), and k = n - Σ_{j=1}^{r-1}(5j-4) - 1
 *            (zero‑based index within the row)
 * Range:     u8 n <= 5, u16 n <= 312, u32 n <= 8910, u64 n <= 8910,
 *            u128 n <= 8910, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* cumulative number of terms in rows 1..r */
static inline idx_t cum_terms(idx_t r)
{
    /* (5r^2 - 3r)/2 */
    return (idx_t)(((__int128)5 * r * r - (__int128)3 * r) / 2);
}

/* coefficient in row r, position k (0‑based) */
static seq_t term_row_k(idx_t r, idx_t k)
{
    /* maximum i such that i*r ≤ k and i ≤ 5 */
    idx_t max_i = k / r;
    if (max_i > 5)
        max_i = 5;

    seq_t sum = 0;
    for (idx_t i = 0; i <= max_i; ++i)
    {
        seq_t comb5 = binomial(5, i);                     /* C(5,i) */
        idx_t sub = k + 4 - i * r;
        if (sub < 4)                /* C(sub,4)=0 for sub<4 */
            continue;
        seq_t comb4 = binomial(sub, 4);                   /* C(sub,4) */
        seq_t prod  = comb5 * comb4;                      /* binomial product */
        if (i & 1)
            sum -= prod;
        else
            sum += prod;
    }
    return sum;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1; return 0 */
        return (seq_t)0;

    /* locate row r such that cum_terms(r-1) < n ≤ cum_terms(r) */
    idx_t r = 1;
    while (cum_terms(r) < n)
        ++r;

    idx_t cum_prev = (r == 1) ? 0 : cum_terms(r - 1);
    idx_t k = n - cum_prev - 1; /* 0‑based index within the row */

    return term_row_k(r, k);
}
