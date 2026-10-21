/*
 * A250157  Number of length 3+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of length‑9 arrays of values 0..n such that every
 * 7‑consecutive block has the property that the maximum of some two
 * elements equals the minimum of the remaining five.
 * Recurrence (order 9):
 *   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
 *          + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
 *   initial values: a(1)=354, a(2)=8377, a(3)=79740, a(4)=455055,
 *   a(5)=1879526, a(6)=6219899, a(7)=17519352, a(8)=43657365, a(9)=98808010
 * This implementation evaluates the recurrence iteratively in O(n)
 * time and works for all supported definitions of seq_t and idx_t. */
#include <stddef.h>

#ifdef __cplusplus
#define SEQ_CONST(x) seq_t(x)
#else
#define SEQ_CONST(x) x
#endif

/* Initial values for n = 1 … 9. */
static const seq_t init[9] = {
    SEQ_CONST(354), SEQ_CONST(8377), SEQ_CONST(79740),
    SEQ_CONST(455055), SEQ_CONST(1879526), SEQ_CONST(6219899),
    SEQ_CONST(17519352), SEQ_CONST(43657365), SEQ_CONST(98808010)
};

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; a(0) is returned as 0. */
    if (n == 0)
        return SEQ_CONST(0);

    if (n <= 9)
        return init[n - 1];

    /* Use a sliding window of the last nine values to apply the recurrence. */
    seq_t win[9];
    for (idx_t i = 0; i < 9; ++i)
        win[i] = init[i];

    for (idx_t i = 9; i < n; ++i) {
        seq_t next =
            SEQ_CONST(9)  * win[8] - SEQ_CONST(36) * win[7] +
            SEQ_CONST(84) * win[6] - SEQ_CONST(126) * win[5] +
            SEQ_CONST(126) * win[4] - SEQ_CONST(84) * win[3] +
            SEQ_CONST(36) * win[2] - SEQ_CONST(9) * win[1] + win[0];

        /* Shift the window left by one. */
        for (idx_t k = 0; k < 8; ++k)
            win[k] = win[k + 1];
        win[8] = next;
    }
    return win[8];
}
