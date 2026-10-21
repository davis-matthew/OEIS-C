/*
 * A297917  Number of n X 2 0..1 arrays with every element equal to 1, 2, 3 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements OEIS A297917 using the listed recurrence.
 * Recurrence:  a(n) = 3*a(n-1) + a(n-2) + 4*a(n-3) + 4*a(n-4)  for n > 5
 * Initial values (n >= 1): 1, 4, 17, 61, 216
 */
#include <stddef.h>  // for size_t

seq_t a(idx_t n)
{
    // Handle known initial cases explicitly
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)61;
    if (n == 5) return (seq_t)216;

    // Use a sliding window of the last four values
    seq_t v1 = (seq_t)216; /* a(n-1) */
    seq_t v2 = (seq_t)61;  /* a(n-2) */
    seq_t v3 = (seq_t)17;  /* a(n-3) */
    seq_t v4 = (seq_t)4;   /* a(n-4) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 3 * v1 + v2 + 4 * v3 + 4 * v4;
        v4 = v3;
        v3 = v2;
        v2 = v1;
        v1 = next;
    }
    return v1;
}
