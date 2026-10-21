/*
 * A293432  Sum of Jacobsthal numbers that divide n.
 * https://oeis.org/A293432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of Jacobsthal numbers that divide n (A001045)
 * Range:     u8 n <= 105, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Count j = 1 once */
    seq_t sum = 1;

    /* Handle small n that are < 3 */
    if (n < 3) return sum;

    /* Jacobsthal numbers from J2 = 1, J3 = 3 upwards */
    seq_t j_prev = 1;  /* J2 */
    seq_t j_curr = 3;  /* J3 */

    if (j_curr <= n && n % (idx_t)j_curr == 0) sum += j_curr;

    while (1) {
        seq_t j_next = j_curr + (seq_t)2 * j_prev;
        if (j_next > n) break;
        j_prev = j_curr;
        j_curr = j_next;
        if (n % (idx_t)j_curr == 0) sum += j_curr;
    }
    return sum;
}
