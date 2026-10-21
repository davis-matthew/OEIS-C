/*
 * A286321  Least possible strictly positive product of the two initial terms
 *          of a Fibonacci-like sequence containing n.
 * https://oeis.org/A286321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_fibs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = least positive product a0*a1 such that the Fibonacci‑like sequence
 * defined by f(0)=a0, f(1)=a1 contains n.  The sequence satisfies
 * f(k) = a0*F_{k-1} + a1*F_k  (F_0=0, F_1=1, F_{-1}=1).
 * We search all Fibonacci indices k (k≥2) with F_k ≤ n and for each
 * possible a1 try to solve a0 = (n – a1*F_k)/F_{k-1} (integer, >0).
 * The trivial construction f(0)=n, f(1)=1 (or vice versa) gives product n,
 * so we initialize the best value with n.
 *
 * The loops are tiny for n≤10000; the algorithm works for any n.
 */

#include <stddef.h>
#include "../utils/compute_fibs.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* not defined for n=0, return 0 */

    seq_t best = (seq_t)n;          /* product n is always achievable */

    /* Fibonacci list up to the first value > n (size ≤ 25 for n≤10000). */
    /* Use a fixed size large enough for all expected n. */
    static const size_t MAX_FIB = 64;
    seq_t F[MAX_FIB];
    size_t len;
    compute_fibs(n, F, &len);

    /* Enumerate all k with F_k ≤ n.  Skip k=1 because F_{k-1}=0. */
    for (size_t k = 2; k < len; ++k) {
        seq_t FK       = F[k];
        seq_t FK_minus = F[k - 1];
        if (FK > (seq_t)n) break;

        /* The maximum a1 such that n – a1*FK > 0. */
        idx_t a1max = ((idx_t)n - 1) / (idx_t)FK;
        for (idx_t a1 = 1; a1 <= a1max; ++a1) {
            seq_t rem = (seq_t)n - (seq_t)a1 * FK;
            if ((rem % FK_minus) != 0) continue;   /* a0 must be integer */
            seq_t a0 = rem / FK_minus;
            if (a0 == 0) continue;                /* must be strictly positive */
            seq_t prod = a0 * (seq_t)a1;
            if (prod < best) best = prod;
        }
    }
    return best;
}
