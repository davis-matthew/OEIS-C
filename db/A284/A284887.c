/*
 * A284887  Number of elements of order n in the Janko group J_1.
 * https://oeis.org/A284887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of elements of order n in the Janko group J1. The
 *            element orders that occur in J1 are {1,2,3,5,6,7,10,11,15,19}.
 *            For n not in this set, a(n)=0
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    /* All known orders and their element counts in J1.  The counts are
       taken from the known conjugacy classes of J1 and are valid for
       any seq_t large enough to hold them. */
    static const struct {
        idx_t ord;
        uint32_t cnt;
    } table[] = {
        {1,  1U},
        {2, 1463U},
        {3, 5852U},
        {5, 11704U},
        {6, 29260U},
        {7, 25080U},
        {10,35112U},
        {11,15960U},
        {15,23408U},
        {19,27720U}
    };
    /* Linear search is trivial here; the table is very small. */
    for (size_t i = 0; i < sizeof(table)/sizeof(*table); ++i) {
        if (table[i].ord == n)
            return (seq_t)table[i].cnt;
    }
    return 0;
}
