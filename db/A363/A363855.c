/*
 * A363855  Number of divisors of 7*n-3 of form 7*k+6.
 * https://oeis.org/A363855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   enumerate_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation: a(n) = number of divisors of 7*n-3 that are congruent to 6 modulo 7.
 * (The same as the number of divisors congruent to 3 modulo 7.)
 */

#include <stdint.h>          /* For UINT64_MAX when needed; types seq_t and idx_t are externally supplied */
#include "../utils/enumerate_div.h"

seq_t a(idx_t n)
{
    /* Compute m = 7*n - 3 */
    idx_t m = 7 * n - 3;
    /* Factor m */
    idx_t primes[64];
    int exps[64];
    int num = 0;

    idx_t rem = m;
    int exp = 0;

    /* Handle factor 2 */
    while ((rem & 1) == 0) {       /* rem % 2 == 0 */
        rem >>= 1;                 /* rem /= 2 */
        ++exp;
    }
    if (exp > 0) {
        primes[num] = 2;
        exps[num] = exp;
        ++num;
    }

    /* Odd factors */
    for (idx_t p = 3; p * p <= rem; p += 2) {
        exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
        }
        if (exp > 0) {
            primes[num] = p;
            exps[num] = exp;
            ++num;
        }
    }

    /* Remaining prime > 1 */
    if (rem > 1) {
        primes[num] = rem;
        exps[num] = 1;
        ++num;
    }

    /* Enumerate divisors and count those ≡ 6 (mod 7) */
    seq_t cnt = 0;
    enumerate_div(0, 1, primes, exps, num, &cnt);
    return cnt;
}
