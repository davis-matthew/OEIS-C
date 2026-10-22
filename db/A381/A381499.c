/*
 * A381499  a(n) = sum of numbers k < n such that 1 < gcd(k,n) < k and rad(k)
 *          does not divide n, where rad = A007947.
 * https://oeis.org/A381499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k < n} k if 1 < gcd(k,n) < k and rad(k) ∤ n (rad =
 *            product of distinct prime divisors of k)
 * Range:     u8 n <= 37, u16 n <= 449, u32 n <= 9644, u64 n <= 9644,
 *            u128 n <= 9644, big any n
 * Helpers:   gcd, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 2) return sum;          /* no k < n to consider */

    for (idx_t k = 2; k < n; ++k) {
        seq_t g = gcd((seq_t)k, (seq_t)n);
        if (g == 1 || g == (seq_t)k) continue;   /* skip coprime or divisor */

        seq_t radk = rad(k);                     /* rad(k) */
        if (((seq_t)n % radk) == 0) continue;    /* rad(k) divides n */

        sum += (seq_t)k;                         /* accumulate */
    }
    return sum;
}
