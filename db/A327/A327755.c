/*
 * A327755  Odd integers k such that binomial(k-1,(k-1)/2) is coprime to k.
 * https://oeis.org/A327755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 9584, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of A327755: odd k such that binomial(k-1,(k-1)/2) is coprime to k. */

#include <stdint.h>
#include <stdbool.h>

/* Returns true if odd integer k satisfies the OEIS condition. */
static bool is_good(uint64_t k)
{
    /* The special case k = 1 passes automatically. */
    if (k == 1)
        return true;

    const uint64_t m = (k - 1) / 2;      /* (k-1)/2 */
    uint64_t temp = k;                   /* copy for factorisation */

    /* Test all odd prime factors up to sqrt(temp). */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            /* Check the base‑p digits of m. */
            uint64_t rem = m;
            uint64_t lim = (p - 1) / 2;
            while (rem > 0) {
                if (rem % p > lim)
                    return false;
                rem /= p;
            }
            /* Remove all occurrences of this prime factor. */
            while (temp % p == 0)
                temp /= p;
        }
    }
    /* If any prime factor > sqrt remains, test it. */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t rem = m;
        uint64_t lim = (p - 1) / 2;
        while (rem > 0) {
            if (rem % p > lim)
                return false;
            rem /= p;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    idx_t count = 0;
    uint64_t k = 1;        /* start at 1 (odd) */

    while (1) {
        if (is_good(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        /* Next odd integer */
        k += 2;
    }
}
