/*
 * A285639  a(n) = n*A117366(n)/lpf(n), where A117366(n) is the smallest prime
 *          larger than all prime factors of n, and lpf is the least prime
 *          factor of n (or 1 if n = 1).
 * https://oeis.org/A285639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A117366(n)/lpf(n)
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/lpf.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 explicitly (lpf(1)=1 by definition) */
    if (n == 1) {
        return (seq_t)2;        /* nextprime(1) = 2, lpf(1) = 1, so a(1)=2 */
    }

    /* Least prime factor of n */
    idx_t lpf_n = lpf(n);

    /* Find the largest prime factor of n */
    idx_t temp = n;
    idx_t maxpf = 0;
    while (temp > 1) {
        idx_t p = lpf(temp);
        if (p > maxpf)
            maxpf = p;
        /* Remove all copies of p */
        do {
            temp /= p;
        } while (temp % p == 0);
    }

    /* Next prime after max prime factor (A117366) */
    uint64_t nextp = next_prime((uint64_t)maxpf);

    /* Compute (n / lpf(n)) * nextp in the result type */
    seq_t part = (seq_t)(n / lpf_n);
    part *= (seq_t)nextp;
    return part;
}
