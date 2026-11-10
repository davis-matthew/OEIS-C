/*
 * A364502  a(n) = A005940(n) / gcd(n, A005940(n)).
 * https://oeis.org/A364502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A005940(n) / gcd(n, A005940(n)), where A005940(n) =
 *            f(n-1,1,1), f(0,i,x)=x, f(2k,i,x)=f(k,i+1,x),
 *            f(2k+1,i,x)=f(k,i,x*prime(i))
 * Range:     u8 n <= 56, u16 n <= 488, u32 n <= 15856, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 100 terms through n = 9940 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A364502_doudna(idx_t n) {
    static const uint64_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
        157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
        239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311
    };
    idx_t m = n - 1;
    seq_t result = 1;
    idx_t i = 1;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)primes[i - 1];
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t d = A364502_doudna(n);
    seq_t g = gcd((seq_t)n, d);
    return d / g;
}
