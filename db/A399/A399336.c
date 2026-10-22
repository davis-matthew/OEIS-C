/*
 * A399336  The binomial matrix of the Jacobsthal numbers, square array read
 *          by ascending antidiagonals.
 * https://oeis.org/A399336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   The entry A(i,j) of the binomial Jacobsthal matrix is * A(i,j) =
 *            Σ_{m=0..d} C(d,m) * s^{d-m} * J(p+m), * where d = |i-j|, p =
 *            min(i,j), s = 1 if j≤i else -1, * and J(k) is the k‑th
 *            Jacobsthal number (J(0)=0, J(1)=1, J(n)=J(n‑1)+2J(n‑2)). * The
 *            1‑dimensional sequence a(n) is obtained by reading th...
 * Range:     i8 n <= 20, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   jacobsthal, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types supplied by the build */
#ifndef IDX_T_DEFINED
#define IDX_T_DEFINED
typedef uint64_t idx_t;
#endif
#ifndef SEQ_T_DEFINED
#define SEQ_T_DEFINED
/* seq_t is provided elsewhere */
#endif

#include <stddef.h>          /* size_t */
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array */
#include "../utils/seq_free_array.h"       /* seq_free_array */
#include "../utils/jacobsthal.h"

/* Compute the binomial coefficient C(n,k) for small arguments
 * using a multiplicative algorithm.  The result is exact because
 * each division is guaranteed to be integer.  n,k are assumed to
 * be < 200, which suffices for the required sequence. */
static seq_t binomial_local(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return (seq_t)1;
    if (k > n)
        return (seq_t)0;
    if (k > n - k)   /* use symmetry */
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;   /* exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal number s such that
     * n ∈ [s(s+1)/2 , (s+1)(s+2)/2).  s is O(√n). */
    idx_t s = 0;
    for (;;) {
        idx_t next = (s + 1) * (s + 2) / 2;
        if (next > n)
            break;
        ++s;
    }
    idx_t offset = n - s * (s + 1) / 2;
    idx_t i = s - offset;   /* row index */
    idx_t j = offset;       /* column index */

    /* Diagonal/parameter calculations */
    idx_t d = (i > j) ? (i - j) : (j - i);
    idx_t p = (i > j) ? j : i;
    int signbase = (j <= i) ? 1 : -1; /* s in the formula */

    /* Pre‑compute Jacobsthal numbers up to p + d for speed */
    idx_t maxJ = p + d;
    seq_t *J = seq_alloc_new_array(maxJ + 1);
    if (J == NULL)
        return (seq_t)0;   /* out of memory – unlikely */
    J[0] = (seq_t)0;
    if (maxJ >= 1)
        J[1] = (seq_t)1;
    for (idx_t k = 2; k <= maxJ; ++k) {
        J[k] = J[k - 1] + (seq_t)2 * J[k - 2];
    }

    seq_t sum = (seq_t)0;
    for (idx_t m = 0; m <= d; ++m) {
        seq_t bin = binomial_local(d, m);
        seq_t val = J[p + m] * bin;
        /* Apply the sign factor s^{d-m} */
        if (signbase == -1 && ((d - m) & 1))
            val = -val;   /* flip sign */
        sum += val;
    }

    seq_free_array(J);
    return sum;
}
