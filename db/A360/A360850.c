/*
 * A360850  Array read by antidiagonals: T(m,n) is the number of (undirected)
 *          paths in the complete bipartite graph K_{m,n}.
 * https://oeis.org/A360850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals from the table * T(m,n) =
 *            Σ_{j=1}^{min(m,n)} j!² · C(m,j) · C(n,j) · (1 + (m+n)/2 – j)
 *            (OEIS A360850) * * For a given index n (1‑based) we first
 *            determine the anti‑diagonal * containing the nth term, then
 *            compute T(m,n) exactly using the * definition above. All
 *            arith...
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 112, u64 n <= 311,
 *            u128 n <= 799, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* outside the offset – not used */

    /* Find smallest s such that (s-1)*s/2 >= n  (s is the diagonal sum) */
    idx_t s = 1;
    while ((unsigned __int128)(s - 1) * s / 2 < n)
        ++s;

    /* Number of terms before this diagonal */
    idx_t prev_total = (idx_t)((unsigned __int128)(s - 2) * (s - 1) / 2);

    /* Zero‑based offset within the diagonal */
    idx_t d  = n - prev_total - 1;
    idx_t m  = 1 + d;      /* first index in the diagonal starts at 1    */
    idx_t k  = s - m;      /* second index follows from the sum s        */

    idx_t min_mk = m < k ? m : k;
    seq_t result = 0;

    /* Incrementally compute j! for j = 1 .. min(m,k) */
    seq_t fact = 1;
    for (idx_t j = 1; j <= min_mk; ++j)
    {
        fact *= (seq_t)j;            /* fact = j!          */
        seq_t fj   = fact;           /* j!                */
        seq_t term = fj * fj;        /* j!²              */
        term *= binom(m, j);         /* × C(m,j)         */
        term *= binom(k, j);         /* × C(k,j)         */
        /* The constant factor (1 + (m+k)/2 - j) = (m + k - 2j + 2)/2 */
        seq_t num = (seq_t)(m + k - 2 * j + 2);   /* numerator */
        term *= num;                 /* multiply by num   */
        term /= 2;                   /* divide by 2       */
        result += term;              /* accumulate sum   */
    }

    return result;
}
