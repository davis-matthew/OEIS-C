/*
 * A343505  a(n) is the least common multiple of the nonzero digits in
 *          factorial base expansion of 1/n.
 * https://oeis.org/A343505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = lcm of the nonzero digits in the factorial base expansion
 *            of 1/n
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 46, u64 n <= 78, u128 n <= 156,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t lcm_seq(seq_t a, seq_t b) {
    /* lcm(a,b) = a/gcd(a,b) * b */
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n) {
    if (n == 1) {
        /* 1/1 = 1, representation has digit 1, so lcm = 1 */
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;      /* start lcm as 1 */
    uint64_t r = 1 % n;        /* remainder for the algorithm (initially 1) */
    uint64_t k = 2;            /* factorial base index starts at 2 */

    while (r != 0) {
        unsigned __int128 tmp = (unsigned __int128)r * k;   /* higher precision for multiplication */
        unsigned long long d = (unsigned long long)(tmp / n); /* digit for position k */
        r = (unsigned long long)(tmp % n);                  /* new remainder */

        if (d > 0) {
            res = lcm_seq(res, (seq_t)d);
        }
        ++k;
    }
    return res;
}
