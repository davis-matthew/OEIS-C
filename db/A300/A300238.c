/*
 * A300238  a(n) = n - A300234(n) = n - A285721(n, phi(n)).
 * https://oeis.org/A300238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A300234(n) = n - A285721(n, phi(n)), where
 *            A285721(n,k) counts the subtraction steps of Euclid's algorithm
 *            on (n,k) until the two numbers are equal
 * Range:     u8 n <= 262, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 114 terms through n = 9395 (2026-11-08)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

static seq_t A300238_steps(seq_t x, seq_t y)
{
    seq_t steps = 0;
    while (x != y) {
        if (x > y) {
            seq_t q = x / y;
            seq_t r = x % y;
            if (r == 0) {
                steps += q - 1;
                break;
            }
            steps += q;
            x = r;
        } else {
            seq_t q = y / x;
            seq_t r = y % x;
            if (r == 0) {
                steps += q - 1;
                break;
            }
            steps += q;
            y = r;
        }
    }
    return steps;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t ph = totient(n);
    return (seq_t)n - A300238_steps((seq_t)n, ph);
}
