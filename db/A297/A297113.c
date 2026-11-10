/*
 * A297113  a(1) = 0, a(2) = 1, after which, a(n) = a(n/2) if n is of the form
 *          4k+2, and otherwise a(n) = 1+a(A252463(n)).
 * https://oeis.org/A297113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 1; a(n) = a(n/2) if n == 2 (mod 4), otherwise
 *            a(n) = 1 + a(A252463(n))
 * Range:     u8 n <= 1618, u16 n <= 12721, u32 n <= 12721, u64 n <= 12721,
 *            u128 n <= 12721, big any n
 * Validated: standard, pass, 113 terms through n = 9922 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t A297113_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    while (!is_prime(p)) --p;
    return p;
}

/* A252463 on indices: a(1)=1, a(2n)=n, odd n: shift prime factors to previous prime */
static idx_t A297113_A252463(idx_t n)
{
    if (n == 1) return 1;
    if ((n & 1) == 0) return n >> 1;
    idx_t m = n;
    idx_t result = 1;
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t base = A297113_prev_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= base;
            }
        }
    }
    if (m > 1) result *= A297113_prev_prime(m);
    return result;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    while (n > 2) {
        if ((n & 3) == 2) {
            n >>= 1;
        } else {
            cnt++;
            n = A297113_A252463(n);
        }
    }
    if (n == 2) cnt++;
    return cnt;
}
