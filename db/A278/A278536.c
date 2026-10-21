/*
 * A278536  First differences of A273324.
 * https://oeis.org/A278536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A273324(n+1) - A273324(n) (periodic sequence with period
 *            6: [3,1,4,1,3,4])
 * Range:     u8 n <= 9996, u16 n <= 9996, u32 n <= 9996, u64 n <= 9996,
 *            u128 n <= 9996, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n) {
    static const seq_t period[6] = {3, 1, 4, 1, 3, 4};
    return period[(n-1) % 6];
}
