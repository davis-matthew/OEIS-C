/*
 * A390675  Number of integer compositions whose first sums are the (2n)-th
 *          composition in standard order.
 * https://oeis.org/A390675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of integer compositions whose first sums are the
 *            (2*n)-th composition in standard order. The function constructs
 *            that composition from the binary representation of 2*n, then
 *            counts the positive integer solutions of the system x_i +
 *            x_{i+1} = C_i , i = 1..k where C is that compositio...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2*n */
    idx_t J = n << 1;

    /* ----- build the composition C from J ----- */
    /* maximum length of a composition is the popcount of J, ≤ 64 */
    uint16_t diff[64];
    int k = 0;                       /* number of parts */
    idx_t tmp = J;
    idx_t pos = 1;                   /* bit positions starting at 1 */
    idx_t prev = 0;

    while (tmp) {
        if (tmp & 1) {
            diff[k++] = (uint16_t)(pos - prev); /* part value */
            prev = pos;
        }
        tmp >>= 1;
        ++pos;
    }

    /* ----- count solutions of the linear system ----- */
    int64_t L = 1;                   /* lower bound for x1   */
    int64_t U = INT64_MAX;           /* upper bound for x1   */
    int sign = 1;                    /* coefficient of x1   */
    int64_t b = 0;                   /* constant term       */

    /* The parts of C are diff[0..k-1] in ascending order of bit positions.
       The composition itself is the reverse of that order. */
    for (int i = k - 1; i >= 0; --i) {
        uint16_t y = diff[i];        /* part C[i] in final order */

        /* impose constraint for current x_i */
        if (sign > 0) {
            int64_t bound = -b + 1;  /* x_i > -b   =>  x1 >= -b+1   */
            if (bound > L) L = bound;
        } else {
            int64_t bound = b - 1;  /* -x1 + b > 0   =>  x1 <= b-1 */
            if (bound < U) U = bound;
        }

        /* compute next coefficient and constant */
        int64_t new_b = (int64_t)y - b;
        sign = -sign;
        b = new_b;
    }

    /* impose constraint for the last part x_{k+1} */
    if (sign > 0) {
        int64_t bound = -b + 1;
        if (bound > L) L = bound;
    } else {
        int64_t bound = b - 1;
        if (bound < U) U = bound;
    }

    /* final count */
    seq_t result;
    if (U >= L)
        result = (seq_t)(U - L + 1);
    else
        result = 0;

    return result;
}
