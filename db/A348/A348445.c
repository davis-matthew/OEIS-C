/*
 * A348445  Irregular triangle read by rows: T(n,k) (n >= 0) is the number of
 *          ways of selecting k objects from n objects arranged on a line with
 *          no two selected objects having unit separation (i.e. having
 *          exactly one object between them).
 * https://oeis.org/A348445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry of the irregular triangle read by rows *
 *            T(n,k) = number of ways of selecting k objects from n arranged
 *            on a line * with no two selected objects having unit separation
 *            (distance 2). * Definition uses binomial coefficient formula: *
 *            T(n,k) = Σ_{i=0}^{k} C(L1-i+1,i)·C(L2-(k-i...
 * Range:     u8 n <= 69, u16 n <= 207, u32 n <= 674, u64 n <= 2449,
 *            u128 n <= 2675, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row index r and column k such that index n lies in that row. */
    idx_t sum = 0;      /* number of entries before the current row       */
    idx_t r = 0;        /* current row being examined                     */

    for (;;) {
        idx_t a_half = r / 2;           /* floor(r/2)        */
        idx_t b_half = (r + 1) / 2;     /* ceil(r/2)         */
        /* kmax = ceil(a_half/2) + ceil(b_half/2) */
        idx_t kmax = (a_half + 1) / 2 + (b_half + 1) / 2;
        idx_t len   = kmax + 1;           /* number of entries in this row */

        if (sum + len > n) {             /* n lies in this row */
            break;
        }
        sum += len;
        ++r;
    }

    idx_t k = n - sum;                   /* column within row r   */

    /* Compute T(r,k) using the independent-vertices formula. */
    idx_t L1 = r / 2;          /* floor(r/2)  */
    idx_t L2 = r - L1;          /* ceil(r/2)   */

    seq_t result = 0;
    for (idx_t i = 0; i <= k; ++i) {
        idx_t j = k - i;               /* k - i      */
        if (i > L1 || j > L2) {       /* out of range */
            continue;
        }
        seq_t term1 = binomial(L1 - i + 1, i);
        seq_t term2 = binomial(L2 - j + 1, j);
        result += term1 * term2;
    }
    return result;
}
