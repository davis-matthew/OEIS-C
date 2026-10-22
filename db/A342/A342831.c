/*
 * A342831  a(n) is the smallest positive integer k such that the
 *          n-dimensional cube [0,k]^n contains at least as many internal
 *          lattice points as external lattice points.
 * https://oeis.org/A342831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k such that * 2*(k-1)^n >=
 *            (k+1)^n. * For n = 1 the value is 3. * * The comparison is
 *            performed using long double logarithms * to avoid overflow and
 *            to work correctly for all seq_t sizes * (uint8_t …, uint64_t,
 *            unsigned __int128 or arbitrary precision). * The bina...
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;        /* a(1) = 3 */
    /* 3*n is an upper bound for all n >= 2 */
    idx_t low  = 1;
    idx_t high = 3 * n;

    const long double ln2 = log(2.0L);

    while (low < high) {
        idx_t mid = (low + high) / 2;
        /* Evaluate inequality:   ln(2) + n*ln(mid-1) >= n*ln(mid+1)  */
        long double left  = ln2 + (long double)n * log((long double)(mid - 1));
        long double right = (long double)n * log((long double)(mid + 1));

        if (left >= right)
            high = mid;
        else
            low  = mid + 1;
    }
    return (seq_t)low;
}
