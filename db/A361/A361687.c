/*
 * A361687  The number of divisors of 2*n^2 which are <=n.
 * https://oeis.org/A361687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ d : d | 2*n^2 and d <= n }|
 * Range:     u8 n <= 15, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   count_div_leq_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_div_leq_n.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Factorisation of n.  We store prime factors of 2*n^2. */
    idx_t t = n;
    size_t pcnt = 0;
    seq_t primes_arr[64];
    int exps_arr[64];

    /* Handle prime 2.  The exponent of 2 in 2*n^2 is 1 + 2*e2
       where e2 is the exponent of 2 in n. */
    int e2 = 0;
    while ((t & 1) == 0) {
        ++e2;
        t >>= 1;
    }
    exps_arr[pcnt] = 1 + 2 * e2;       /* exponent of 2 in 2*n^2 */
    primes_arr[pcnt++] = 2;

    /* Handle odd primes in n.  In 2*n^2 each odd prime p appears
       with exponent 2*e where e is its exponent in n. */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                ++e;
                t /= p;
            }
            exps_arr[pcnt] = 2 * e;
            primes_arr[pcnt++] = p;
        }
    }
    if (t > 1) {                     /* remaining prime factor */
        exps_arr[pcnt] = 2;
        primes_arr[pcnt++] = t;
    }

    seq_t cnt = 0;
    count_div_leq_n(primes_arr, exps_arr, pcnt, 0, (seq_t)1, n, &cnt);
    return cnt;
}
