/*
 * A259334  Triangle read by rows: T(n, k) = k * (n - 1)! * n^(n - k - 1) / (n
 *          - k)!.
 * https://oeis.org/A259334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th element of the triangle read by rows, where
 *            T(r,k) = k * (r-1)! * r^(r-k-1) / (r-k)! (1 ≤ k ≤ r)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 57, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---- helper: integer power (base^(exp)), base, exp are non‑negative ---- */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        exp >>= (idx_t)1;
        if (exp > (idx_t)0) {
            base *= base;
        }
    }
    return result;
}

/* ---- helper: factorial of m (m! ) ----------------------------------- */
static seq_t fact_upto(idx_t m)
{
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= m; ++i) {
        res *= (seq_t)i;
    }
    return res;
}

/* ---- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the row r such that 1 + 2 + ... + r ≥ n.
       cum = r*(r+1)/2 after the loop. */
    idx_t r = (idx_t)1;
    idx_t cum = (idx_t)1;                 /* cumulative count after row r */
    while (cum < n) {
        ++r;
        cum += r;                         /* cum = r*(r+1)/2 */
    }

    /* Number of elements before row r: (r-1)*r/2 = cum - r */
    idx_t before = cum - r;

    /* Column index within the row (1‑based) */
    idx_t k = n - before;                 /* 1 ≤ k ≤ r */

    /* Special case: k == r → T(r,r) = (r-1)! */
    if (k == r) {
        return fact_upto(r - (idx_t)1);
    }

    /* Compute product = (r-k+1) * (r-k+2) * ... * (r-1)
       which equals (r-1)! / (r-k)! */
    seq_t product = (seq_t)1;
    for (idx_t i = r - k + (idx_t)1; i <= r - (idx_t)1; ++i) {
        product *= (seq_t)i;
    }

    /* exponent for r^{...} is r - k - 1 (non‑negative here) */
    idx_t exp = r - k - (idx_t)1;
    seq_t pow_val = pow_uint((seq_t)r, exp);

    /* Final result: k * product * pow_val */
    seq_t result = product;
    result *= pow_val;
    result *= (seq_t)k;

    return result;
}
