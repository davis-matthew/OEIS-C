/*
 * A318618  a(n) is the number of rooted forests on n nodes that avoid the
 *          patterns 321, 2143, and 3142.
 * https://oeis.org/A318618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*a(n-1) - n*(n-1)/2*a(n-2) with a(0) = 1, a(1) = 1
 *            This implements the explicit recurrence given in the OEIS entry.
 *            The computation is done iteratively in O(n) time and uses only
 *            integer arithmetic on the seq_t type, which may be an
 *            arbitrary-precision integer class or a fixed-width uns...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t an;              /* current a(i) */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute t = i*(i-1)/2 */
        seq_t t = (seq_t)i * (seq_t)(i - 1);
        t = t / (seq_t)2;

        /* a(n) = 2*i*a1 - t*a0 */
        an = ((seq_t)2 * (seq_t)i * a1) - (t * a0);

        /* shift for next iteration */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
