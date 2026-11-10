/*
 * A249826  Permutation of natural numbers: a(n) =
 *          A078898(A003961(A003961(A003961(2*n)))).
 * https://oeis.org/A249826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A078898(A003961(A003961(A003961(2*n)))); the argument is
 *            7*S with S = A003961^3(n), so a(n) = #{ j <= S : gcd(j,30)=1 }
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Validated: standard, pass, 99 terms through n = 119 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A249826_shift3(uint64_t p)
{
    p = next_prime(p);
    p = next_prime(p);
    p = next_prime(p);
    return p;
}

seq_t a(idx_t n)
{
    static const int cnt[30] = {
        0,1,1,1,1,1,1,2,2,2,
        2,3,3,4,4,4,4,5,5,6,
        6,6,6,7,7,7,7,7,7,8
    };
    seq_t S = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = A249826_shift3(p);
            while (m % p == 0) {
                m /= p;
                S *= (seq_t)q;
            }
        }
    }
    if (m > 1) {
        S *= (seq_t)A249826_shift3(m);
    }
    seq_t r = S % 30;
    return (S / 30) * 8 + (seq_t)cnt[(int)(long long)r];
}
