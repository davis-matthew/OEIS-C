/*
 * A350528  Triangle read by rows: T(n,k) is the number of labeled
 *          quasi-threshold graphs on vertex set [n] with k components, for n
 *          >= 1 and 1 <= k <= n.
 * https://oeis.org/A350528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=k..n} (-1)^{n-j} S(n,j) * k * binomial(j,k) *
 *            j^{j-k-1} where S(n,j) is the Stirling numbers of the second
 *            kind. a(n) is obtained by flattening the triangle row by row:
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=4, a(5)=3, a(6)=1, ... This
 *            implementation builds the required row of Stirling numb...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required utilities */
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/*  External integer types supplied by the build system   */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /*  For the offset; callers will never pass n==0, but guard anyway  */
    if (n == 0) return 0;

    /*  Find the row r such that r*(r+1)/2 >= n  */
    idx_t r = 1;
    while (r * (r + 1ULL) / 2ULL < n) ++r;

    /*  k is the component index within the row  */
    idx_t prevRows = (r - 1ULL) * r / 2ULL;
    idx_t offset   = n - prevRows - 1ULL;   /* 0‑based offset in the row  */
    idx_t k        = offset + 1ULL;         /* 1‑based component index      */

    /*  Allocate two rows for Stirling numbers: prev and cur  */
    seq_t *prev = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!prev) return 0;   /*  allocation failure  */

    /*  S(0,0) = 1  */
    prev[0] = 1;

    /*  Build up to S(r,j)  */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t *cur = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
        if (!cur) {
            free(prev);
            return 0;
        }
        cur[0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            cur[j] = prev[j] * (seq_t)j + prev[j - 1];
        }
        free(prev);
        prev = cur;
    }

    /*  Alternating sum over j = k .. r  */
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t j = k; j <= r; ++j) {
        seq_t term;
        if (j == k) {
            /*  For j == k, j^{j-k-1} = 1/j and k = j, so the product
                simplifies to S(r,j).  */
            term = prev[j];
        } else {
            seq_t pow = big_pow((seq_t)j, (seq_t)(j - k - 1ULL));
            term = prev[j] * binomial(j, k) * (seq_t)k * pow;
        }

        /*  Add or subtract depending on parity of (r - j)  */
        if (((r - j) & 1ULL) == 0ULL)
            pos += term;
        else
            neg += term;
    }

    free(prev);

    return pos - neg;   /*  pos should be >= neg  */
}
