/*
 * A363612  Number of iterations of phi(x) at n needed to reach a square.
 * https://oeis.org/A363612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of φ(x) until x becomes a perfect
 *            square
 * Range:     u8 n <= 63, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   calc_phi, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t r = floor_sqrt(x);
    if (r * r == x) {
        return 0;
    }
    seq_t cnt = 0;
    while (true) {
        x = calc_phi(x);
        ++cnt;
        r = floor_sqrt(x);
        if (r * r == x) {
            return cnt;
        }
    }
}
