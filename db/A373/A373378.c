/*
 * A373378  a(n) = gcd(A003415(n), A059975(n)), where A003415 is the
 *          arithmetic derivative and A059975 is fully additive with a(p) =
 *          p-1.
 * https://oeis.org/A373378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A059975(n)) where A003415 is the
 *            arithmetic derivative and A059975 is the fully additive function
 *            with a(p)=p-1
 * Range:     u8 n <= 251, u16 n <= 9092, u32 n <= 9092, u64 n <= 9092,
 *            u128 n <= 9092, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* a(1) = 0 */
    if (n <= 1) return (seq_t)0;

    uint64_t n64 = (uint64_t)n;
    seq_t nseq = (seq_t)n64;

    seq_t derivative = 0;
    seq_t additive   = 0;

    uint64_t temp = n64;

    /* trial division – fast enough for n up to ~10⁹,
       and acceptable within the time limits for n up to ~10⁴ */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pseq = (seq_t)p;
            seq_t est   = (seq_t)e;
            derivative += (nseq / pseq) * est;
            additive   += est * (pseq - (seq_t)1);
        }
    }

    /* remaining prime factor greater than sqrt(original n) */
    if (temp > 1) {
        seq_t pseq = (seq_t)temp;
        seq_t est  = 1;
        derivative += (nseq / pseq) * est;
        additive   += est * (pseq - (seq_t)1);
    }

    return gcd(derivative, additive);
}
