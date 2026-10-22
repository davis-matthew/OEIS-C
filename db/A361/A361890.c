/*
 * A361890  a(n) = S(7,n), where S(r,n) = Sum_{k = 0..floor(n/2)} (
 *          binomial(n,k) - binomial(n,k-1) )^r.
 * https://oeis.org/A361890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} ( binomial(n,k) - binomial(n,k-1)
 *            )^7
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t half = n / 2;
    /* Allocate array for binomial coefficients up to half */
    seq_t *bin = (seq_t *)malloc((half + 1) * sizeof(seq_t));
    if (!bin) return 0;                 /* allocation failure */

    bin[0] = 1;
    for (idx_t k = 1; k <= half; ++k) bin[k] = 0;

    /* Build Pascal's triangle row by row, keeping only first half+1 entries */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t maxk = (i < half) ? i : half;
        for (idx_t k = maxk; k > 0; --k) {
            bin[k] = bin[k] + bin[k - 1];
        }
    }

    /* Sum the 7th powers of the differences */
    seq_t sum = 0;
    for (idx_t k = 0; k <= half; ++k) {
        seq_t left  = (k == 0) ? 0 : bin[k - 1];
        seq_t delta = bin[k] - left;

        /* delta^7 computed as (((delta^2)^2 * delta^2) * delta) */
        seq_t d2 = delta * delta;
        seq_t d4 = d2 * d2;
        seq_t d6 = d4 * d2;
        seq_t d7 = d6 * delta;

        sum += d7;
    }

    free(bin);
    return sum;
}
