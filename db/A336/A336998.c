/*
 * A336998  a(n) = n! * Sum_{d|n} 3^(d - 1) / d!.
 * https://oeis.org/A336998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} 3^(d - 1) / d!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   factorial_loop, pow3_loop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 5, a(3) = 15, ... */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factorial_loop.h"
#include "../utils/pow3_loop.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;  /* Undefined for the sequence offset, but harmless */
    }
    seq_t fn = factorial_loop(n);
    seq_t result = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t fd1 = factorial_loop(d1);
            seq_t p1  = pow3_loop(d1 - 1);
            result += (fn / fd1) * p1;

            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t fd2 = factorial_loop(d2);
                seq_t p2  = pow3_loop(d2 - 1);
                result += (fn / fd2) * p2;
            }
        }
    }
    return result;
}
