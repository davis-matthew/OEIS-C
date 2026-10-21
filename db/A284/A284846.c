/*
 * A284846  Number of elements of order n in simple Mathieu group M_22.
 * https://oeis.org/A284846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of elements of order n in the simple Mathieu group
 *            M_22
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the number of elements of order n in M_22 */
seq_t a(idx_t n)
{
    /* Conjugacy classes of M_22: (order, number of elements) */
    static const struct { idx_t order; seq_t count; } classes[] = {
        {1,  (seq_t)1},
        {2,  (seq_t)1155},
        {3,  (seq_t)12320},
        {4,  (seq_t)41580},
        {5,  (seq_t)88704},
        {6,  (seq_t)36960},
        {7,  (seq_t)126720},
        {8,  (seq_t)55440},
        {11, (seq_t)80640}
    };
    /* Search for the matching order */
    for (unsigned i = 0; i < sizeof(classes)/sizeof(classes[0]); ++i) {
        if (n == classes[i].order) {
            return classes[i].count;
        }
    }
    return (seq_t)0;   /* no elements of this order */
}
