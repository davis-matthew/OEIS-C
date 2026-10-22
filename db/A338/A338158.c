/*
 * A338158  Triangle read by rows: T(n,k) is the coefficient of x^k in the ZZ
 *          polynomial of the hexagonal graphene flake O(3,3,n).
 * https://oeis.org/A338158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle of T(n,k) defined by T(n,k) =
 *            Σ_{l=0..9} C(k+l, k) * ( C(9,k+l)C(n,k+l) + (10·C(7,k+l-2) –
 *            C(6,k+l-2)) · C(n+1,k+l) + (20·C(5,k+l-4)+C(3,k+l-3)–C(3,k+l-5))
 *            · C(n+2,k+l) + (10·C(3,k+l-6)+C(2,k+l-5)+C(3,k+l-5)) ·
 *            C(n+3,k+l) + C(2,k+l-7) · C(n+4,k+l) ) with C(a,b)=0 if...
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

static inline seq_t binom_range(int n, int k)
{
    if (k < 0 || k > n) return (seq_t)0;
    return binomial((idx_t)n, (idx_t)k);
}

static inline seq_t T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (int l = 0; l <= 9; ++l)
    {
        seq_t coef = binomial((idx_t)(k + l), (idx_t)k);
        int kk_l = (int)(k + l);

        seq_t term = 0;

        /* term 1 */
        term += binom_range(9, kk_l) * binom_range((int)n, kk_l);

        /* term 2 */
        seq_t coeff2 = 10 * binom_range(7, kk_l - 2) - binom_range(6, kk_l - 2);
        term += coeff2 * binom_range((int)n + 1, kk_l);

        /* term 3 */
        seq_t coeff3 = 20 * binom_range(5, kk_l - 4)
                      + binom_range(3, kk_l - 3)
                      - binom_range(3, kk_l - 5);
        term += coeff3 * binom_range((int)n + 2, kk_l);

        /* term 4 */
        seq_t coeff4 = 10 * binom_range(3, kk_l - 6)
                      + binom_range(2, kk_l - 5)
                      + binom_range(3, kk_l - 5);
        term += coeff4 * binom_range((int)n + 3, kk_l);

        /* term 5 */
        term += binom_range(2, kk_l - 7) * binom_range((int)n + 4, kk_l);

        sum += coef * term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1, behaviour for n<1 irrelevant */

    idx_t pos = n;
    idx_t row;
    if (pos <= 4) row = 1;
    else if (pos <= 10) row = 2;
    else if (pos <= 18) row = 3;
    else if (pos <= 27) row = 4;
    else
        row = (pos + 22) / 10;   /* ceil((pos+13)/10) for rows >=5 */

    idx_t before;
    if (row == 1) before = 0;
    else if (row == 2) before = 4;
    else if (row == 3) before = 10;
    else if (row == 4) before = 18;
    else before = 27 + 10 * (row - 5);

    idx_t k = pos - before - 1;

    return T(row, k);
}
