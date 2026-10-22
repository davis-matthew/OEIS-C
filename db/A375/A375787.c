/*
 * A375787  a(n) is the number of solutions of n*x*y*z = (x + n)*(y + n)*(z +
 *          n), 0 < x <= y <= z.
 * https://oeis.org/A375787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer triples 0 < x <= y <= z satisfying
 *            n*x*y*z = (x + n)*(y + n)*(z + n)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;
    const unsigned long long N = (unsigned long long)n;
    unsigned long long count = 0;

    for (idx_t x = 2; ; ++x) {
        /* A = (N-1)*x - N  ;  D = A*y - N*x - N*N  */
        unsigned long long A = (N - 1) * (unsigned long long)x - N;
        if (A <= 0) continue;          /* for n=2, x=2 gives A==0 */

        /* minimal y that makes D>0:  y > (N*x + N^2)/A */
        unsigned long long val1 = N * (unsigned long long)x + N * N;
        unsigned long long y_start = val1 / A + 1;
        if (y_start < (unsigned long long)x) y_start = x;

        /* quick check: if the first admissible y already gives z<y, stop */
        unsigned long long D_start = A * y_start - N * (unsigned long long)x - N * N;
        if (D_start <= 0) continue;    /* should not happen */

        unsigned long long numer_start = N * (unsigned long long)(x + N) *
                                         (unsigned long long)(y_start + N);
        unsigned long long z_floor_start = numer_start / D_start;
        if (z_floor_start < y_start)   /* no solutions for this and larger x */
            break;

        /* inner loop over y */
        for (idx_t y = y_start; ; ++y) {
            unsigned long long D = A * (unsigned long long)y -
                                   N * (unsigned long long)x - N * N;
            if (D == 0) continue;      /* should not happen */

            unsigned long long numer = N * (unsigned long long)(x + N) *
                                       (unsigned long long)(y + N);
            unsigned long long z_floor = numer / D;
            if (z_floor < (unsigned long long)y)      /* z will only get smaller */
                break;

            if (numer % D == 0) {          /* integer z */
                unsigned long long z = numer / D;
                /* ordering condition already holds because we ensured z>=y */
                count++;
            }
        }
    }
    return (seq_t)count;
}
