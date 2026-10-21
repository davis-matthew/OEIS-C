/*
 * A280415  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 262", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) – p_n (p_n are the coefficients of the
 *            generating function numerator; for n > 31, p_n = 0). This
 *            recurrence generates OEIS sequence A280415
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int p[32] = {
    -1, /* x^0  */
    -3, /* x^1  */
     0, /* x^2  */
    -2, /* x^3  */
    -1, /* x^4  */
    -1, /* x^5  */
     3, /* x^6  */
     1, /* x^7  */
     0, /* x^8  */
    -2, /* x^9  */
    15, /* x^10 */
    55, /* x^11 */
     3, /* x^12 */
     3, /* x^13 */
     3, /* x^14 */
     3, /* x^15 */
     3, /* x^16 */
     3, /* x^17 */
     3, /* x^18 */
     3, /* x^19 */
     3, /* x^20 */
     3, /* x^21 */
     3, /* x^22 */
     3, /* x^23 */
     3, /* x^24 */
     3, /* x^25 */
     3, /* x^26 */
     3, /* x^27 */
     3, /* x^28 */
     1, /* x^29 */
     4, /* x^30 */
    12  /* x^31 */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for values up to n */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq)
        return (seq_t)0;   /* out of memory – unlikely for n ≤ 10000 */

    seq[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t prev = (i >= 2) ? seq[i - 2] : (seq_t)0;
        seq_t term = (seq_t)4 * prev;

        int pk = (i < 32) ? p[i] : 0;   /* p_i is 0 for i ≥ 32 */

        if (pk >= 0)
            term -= (seq_t)pk;
        else
            term += (seq_t)(-pk);

        seq[i] = term;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
