/*
 * A379148  a(n) is the number of iterations of the function x --> 2*x + 1
 *          such that x remains prime, starting from A005384(n).
 * https://oeis.org/A379148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k>=1 with prime 2^k*(p+1)-1, consecutive from
 *            k=1, where p = A005384(n) (nth Sophie Germain prime); computed
 *            by search
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-10)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

static idx_t A379148_last_n = 0;
static seq_t A379148_last_p = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t count;
    seq_t p;
    if (A379148_last_n != 0 && n >= A379148_last_n) {
        count = A379148_last_n;
        p = A379148_last_p;
    } else {
        count = 1;
        p = 2;
    }
    while (count < n) {
        p = (p == 2) ? 3 : p + 2;
        if (is_prime_seq(p)) {
            seq_t q = 2 * p + 1;
            if (is_prime_seq(q)) count++;
        }
    }
    A379148_last_n = n;
    A379148_last_p = p;

    seq_t x = p;
    seq_t steps = 0;
    while (true) {
        seq_t y = 2 * x + 1;
        if ((y >> 1) != x) break;
        if (!is_prime_seq(y)) break;
        steps++;
        x = y;
    }
    return steps;
}
