/*
 * A395639  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are distinct Fibonacci numbers with y < z.
 * https://oeis.org/A395639
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of distinct Fibonacci * numbers
 *            with y < z such that 2*x^2 + y*z = n
 * Range:     u8 n <= 24, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 8) return 0;          /* values below offset are irrelevant */

    const seq_t N = (seq_t)n;      /* convert to seq_t */
    const size_t MAX_FIBS = 200;   /* safe upper bound for fibs <= N */

    seq_t fibs[MAX_FIBS];
    size_t fibcnt = 0;

    /* generate unique Fibonacci numbers up to N */
    seq_t f0 = 1, f1 = 1;
    fibs[fibcnt++] = f0;          /* 1 appears only once */
    while (1) {
        seq_t f2 = f0 + f1;
        if (f2 > N) break;
        fibs[fibcnt++] = f2;
        f0 = f1;
        f1 = f2;
    }

    seq_t count = 0;

    for (size_t ix = 0; ix < fibcnt; ++ix) {
        seq_t x   = fibs[ix];
        seq_t two_x_sq = x * x + x * x;          /* 2*x^2 */
        if (two_x_sq >= N) continue;
        seq_t target = N - two_x_sq;             /* need y*z = target */

        for (size_t iy = 0; iy < fibcnt; ++iy) {
            if (iy == ix) continue;             /* y distinct from x */
            seq_t y = fibs[iy];
            for (size_t iz = iy + 1; iz < fibcnt; ++iz) {
                if (iz == ix) continue;         /* z distinct from x */
                seq_t z = fibs[iz];
                /* y < z guaranteed by index order because fibs is sorted */
                seq_t prod = y * z;
                if (prod == target) ++count;
            }
        }
    }

    return count;
}
