/*
 * A303428  Number of ways to write n as x*(3*x-2) + y*(3*y-2) + 3^u + 3^v,
 *          where x,y,u,v are integers with x <= y and 0 <= u <= v.
 * https://oeis.org/A303428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as * x*(3*x-2) + y*(3*y-2)
 *            + 3^u + 3^v * where x <= y, 0 <= u <= v, and x,y,u,v are
 *            integers * (x and y may be negative)
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation enumerates all feasible values
 * for x, y, u, v using only integer arithmetic. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(1) = 0 */

    /* ----- precompute powers of 3 up to n ----- */
    seq_t pow3[20];
    size_t pc = 0;
    pow3[pc++] = 1;
    while (pow3[pc-1] <= n / 3) {
        pow3[pc] = pow3[pc-1] * 3;
        pc++;
    }

    /* ----- find bounds for x and y ----- */
    idx_t pos_limit = 0;
    for (idx_t x = 1;; ++x) {                     /* x >= 0 */
        seq_t gx = 3*(seq_t)x*(seq_t)x - 2*(seq_t)x;
        if (gx > n) break;
        pos_limit = x;
    }

    idx_t neg_limit = 0;
    for (idx_t y = 1;; ++y) {                     /* y = -x > 0 */
        seq_t gy = 3*(seq_t)y*(seq_t)y + 2*(seq_t)y;
        if (gy > n) break;
        neg_limit = y;
    }

    /* ----- count representations ----- */
    seq_t total = 0;

    for (long long x = -(long long)neg_limit; x <= (long long)pos_limit; ++x) {
        seq_t gx = 3*(seq_t)x*(seq_t)x - 2*(seq_t)x;
        if (gx > n) continue;

        long long y_start = x > -(long long)neg_limit ? x : -(long long)neg_limit;
        for (long long y = y_start; y <= (long long)pos_limit; ++y) {
            seq_t gy = 3*(seq_t)y*(seq_t)y - 2*(seq_t)y;
            seq_t sum = gx + gy;
            if (sum > n) continue;

            seq_t rem = n - sum;

            for (size_t ui = 0; ui < pc; ++ui) {
                seq_t pu = pow3[ui];
                if (pu > rem) break;

                seq_t rem2 = rem - pu;
                for (size_t vi = ui; vi < pc; ++vi) {
                    if (pow3[vi] == rem2) {
                        total++;
                        break;
                    }
                }
            }
        }
    }

    return total;
}
