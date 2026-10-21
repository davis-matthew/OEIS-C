/*
 * A270866  a(n) = 3*p^5+12*p^4+44*p^3+170*p^2+707*p+2455
 *          +(4*p^2+44*p+291)*gcd(p-1,3) +(p^2+19*p+135)*gcd(p-1,4)
 *          +(3*p+31)*gcd(p-1,5) +4*gcd(p-1,7) +5*gcd(p-1,8) +gcd(p-1,9),
 *          where p = prime(n).
 * https://oeis.org/A270866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*p^5+12*p^4+44*p^3+170*p^2+707*p+2455 *
 *            +(4*p^2+44*p+291)*gcd(p-1,3) * +(p^2+19*p+135)*gcd(p-1,4) *
 *            +(3*p+31)*gcd(p-1,5) * +4*gcd(p-1,7)+5*gcd(p-1,8)+gcd(p-1,9) *
 *            where p = prime(n)
 * Range:     u8 none, u16 n <= 3, u32 n <= 18, u64 n <= 753, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime, small_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/small_gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* get the nth prime (1‑based) */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p4 = p3 * p;
    seq_t p5 = p4 * p;

    seq_t g3 = small_gcd(p - 1, 3);
    seq_t g4 = small_gcd(p - 1, 4);
    seq_t g5 = small_gcd(p - 1, 5);
    seq_t g7 = small_gcd(p - 1, 7);
    seq_t g8 = small_gcd(p - 1, 8);
    seq_t g9 = small_gcd(p - 1, 9);

    seq_t val = 3 * p5 + 12 * p4 + 44 * p3 + 170 * p2 + 707 * p + 2455;
    val += (4 * p2 + 44 * p + 291) * g3;
    val += (p2 + 19 * p + 135) * g4;
    val += (3 * p + 31) * g5;
    val += 4 * g7;
    val += 5 * g8;
    val += g9;

    return val;
}
