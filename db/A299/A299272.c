/*
 * A299272  Coordination sequence for "flu" 3D uniform tiling formed from
 *          tetrahedra, rhombicuboctahedra, and cubes.
 * https://oeis.org/A299272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) - 3*a(n-6) + a(n-9) for n > 9. Initial values:
 *            a(0)=1, a(1)=6, a(2)=18, a(3)=37, a(4)=63, a(5)=99, a(6)=142,
 *            a(7)=189, a(8)=249, a(9)=317
 * Range:     u8 n <= 8, u16 n <= 130, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* handle the first ten terms directly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)6;
        case 2: return (seq_t)18;
        case 3: return (seq_t)37;
        case 4: return (seq_t)63;
        case 5: return (seq_t)99;
        case 6: return (seq_t)142;
        case 7: return (seq_t)189;
        case 8: return (seq_t)249;
        case 9: return (seq_t)317;
        default: break;               /* n > 9 */
    }

    /* compute via the recurrence.  The term count is small (<=10000)
       so an O(n) pass per call is fast enough. */
    size_t sz = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr)
        return (seq_t)0;   /* fallback for OOM; not expected */

    /* initialise the known values */
    arr[0] =  1; arr[1] =  6; arr[2] = 18; arr[3] = 37; arr[4] = 63;
    arr[5] = 99; arr[6] = 142; arr[7] = 189; arr[8] = 249; arr[9] = 317;

    /* iterate from 10 up to n using the linear recurrence */
    for (size_t i = 10; i <= n; ++i) {
        arr[i] = 3 * arr[i-3] - 3 * arr[i-6] + arr[i-9];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
