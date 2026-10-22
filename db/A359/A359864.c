/*
 * A359864  a(n) is the number of solutions to the congruence x^y == y^x (mod
 *          n) where 0 <= x,y <= n.
 * https://oeis.org/A359864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x, y) in [0,n]^2 : x^y ≡ y^x (mod n) }
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Ensure n >= 1, but works for any n >= 0 */
    const seq_t n_seq = (seq_t)n;
    /* Array holding y^x (mod n) for current exponent x */
    const size_t sz = (size_t)(n + 1);
    seq_t *pow_y = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!pow_y) return 0;  /* allocation failure, return 0 */

    /* Initialize for x = 0: y^0 = 1 (mod n) */
    for (size_t i = 0; i < sz; ++i)
        pow_y[i] = (seq_t)1 % n_seq;

    seq_t count = 0;

    for (idx_t x = 0; x <= n; ++x) {
        seq_t powx = (seq_t)1 % n_seq;          /* x^0 */
        for (idx_t y = 0; y <= n; ++y) {
            if (y > 0)                         /* compute x^y incrementally */
                powx = (powx * (seq_t)x) % n_seq;
            if (powx == pow_y[(size_t)y])      /* test congruence */
                ++count;
        }
        /* Update pow_y: y^(x+1) = y^x * y (mod n) */
        for (idx_t y = 0; y <= n; ++y) {
            pow_y[(size_t)y] = (pow_y[(size_t)y] * (seq_t)y) % n_seq;
        }
    }

    free(pow_y);
    return count;
}
