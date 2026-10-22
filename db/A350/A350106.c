/*
 * A350106  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} j * floor(n/j)^k.
 * https://oeis.org/A350106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array * T(n,k) =
 *            Σ_{j=1..n} j * ⌊n/j⌋^k along antidiagonals, starting at * T(1,1)
 *            and moving downwards along each diagonal. In other words, * the
 *            n-th term corresponds to the pair (i,j) where i+j-1 is the *
 *            diagonal number, i counts from the top of the diago...
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: fast binary exponentiation for arbitrary integer types */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Helper: compute the square array entry T(n,k) */
static inline seq_t T_value(idx_t n, idx_t k) {
    seq_t total = 0;
    idx_t l = 1;
    while (l <= n) {
        idx_t q = n / l;        /* floor(n/l) for this block */
        idx_t r = n / q;        /* last j with same floor value */
        /* sum of j from l to r: (l+r)*(r-l+1)/2 */
        seq_t sum_j = (seq_t)(l + r) * (seq_t)(r - l + 1) / 2;
        seq_t pow_qk = pow_seq((seq_t)q, k);
        total += sum_j * pow_qk;
        l = r + 1;
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Find the diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        d++;
        cumulative += d;
    }
    /* Position t within this diagonal (1-indexed) */
    idx_t t = n - (cumulative - d);
    /* Indices in the square array: i = t, j = d - t + 1 */
    idx_t i = t;
    idx_t j = d - t + 1;
    return T_value(i, j);
}
