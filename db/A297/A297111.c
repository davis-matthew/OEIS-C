/*
 * A297111  Möbius transform of A005187, where A005187(n) = 2n - (number of
 *          1's in binary representation of n).
 * https://oeis.org/A297111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2*d - popcount(d)) * mu(n/d)
 * Range:     u8 n <= 130, u16 n <= 9992, u32 n <= 9992, u64 n <= 9992,
 *            u128 n <= 9992, big any n
 * Helpers:   dfs_const
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  μ(m) is the Möbius function.  popcount(x) is the number of 1's
 *  in the binary representation of x.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/dfs_const.h"

/* Main function computing a(n). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 */

    /* Factorise n (trial division, sufficient for n <= ~1e9). */
    uint64_t primes[16];
    int   exp[16];
    int   pfcnt = 0;

    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            primes[pfcnt] = p;
            exp[pfcnt]    = e;
            ++pfcnt;
        }
    }
    if (t > 1) {
        primes[pfcnt] = t;
        exp[pfcnt]    = 1;
        ++pfcnt;
    }

    long long sum = 0;  /* signed accumulator */
    dfs_const(primes, exp, pfcnt, 0, 1, false, 0, &sum);

    return (seq_t)sum;
}
