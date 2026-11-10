/*
 * A400165  The largest prime dividing A380459(n), with a(1) = 1 by
 *          convention.
 * https://oeis.org/A400165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Max_{d|n, d in A246655} A000040(A235224(n/d)); the max is
 *            at d = smallest prime factor of n. a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9670 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A380459_gpf_helper(idx_t n)
{
    static const uint64_t pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t p = n;
    uint64_t m, P, lim;
    unsigned k;
    uint64_t q;
    if (n <= 1)
        return (seq_t)1;
    /* smallest prime factor */
    p = n;
    for (q = 2; q <= n / q; q += (q == 2 ? 1 : 2)) {
        if (n % q == 0) {
            p = q;
            break;
        }
    }
    m = n / p;
    /* k = largest k with primorial(k-1) <= m */
    k = 1;
    P = 2;
    lim = ~(uint64_t)0;
    while (P <= m) {
        uint64_t nx = pr[k];
        k++;
        if (P > lim / nx)
            break;
        P *= nx;
    }
    return (seq_t)pr[k - 1];
}

seq_t a(idx_t n)
{
    return A380459_gpf_helper(n);
}
