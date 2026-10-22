/*
 * A359627  Irregular table read by rows; the n-th row lists the divisors d of
 *          2*n such that the binary expansions of d and 2*n have no common
 *          1-bit.
 * https://oeis.org/A359627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate rows of the table and the qualifying divisors
 * of 2*row.  Return the nth element in row-major order.  A divisor
 * d of m=2*row is accepted if (d & m) == 0. */
#include <stdint.h>
#include <stddef.h>

static void insertion_sort(uint64_t *a, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        uint64_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; a(0) is undefined */

    idx_t count = 0;        /* number of qualifying divisors seen so far */

    for (idx_t row = 1; ; ++row) {   /* walk rows until we find the nth term */
        uint64_t m = row << 1;       /* m = 2*row, fits in 64 bits for the tested ranges */

        /* collect qualifying divisors of m */
        uint64_t divs[256];
        size_t sz = 0;

        for (uint64_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                if ((d & m) == 0) divs[sz++] = d;
                uint64_t e = m / d;
                if (e != d && (e & m) == 0) divs[sz++] = e;
            }
        }

        if (sz == 0) continue;       /* no terms in this row */

        insertion_sort(divs, sz);     /* ensure increasing order */

        for (size_t i = 0; i < sz; ++i) {
            ++count;
            if (count == n) return (seq_t)divs[i];
        }
    }
}
