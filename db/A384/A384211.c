/*
 * A384211  a(n) is the number of distinct ways of representing n in any
 *          integer base >= 2 using only prime digits.
 * https://oeis.org/A384211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct representations of n in any integer
 *            base >= 2 where every digit is a prime number (2,3,5,7,...). The
 *            representation [n] counts once if n is prime, regardless of the
 *            number of bases ≥ n+1. The function returns a(n) as a seq_t
 * Range:     u8 n <= 2476, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* a(n): number of representations using only prime digits */
seq_t a(idx_t n)
{
    /* quick answer for n < 2 */
    if (n < 2)
        return (seq_t)0;

    /* static cache of primality up to the largest n seen so far */
    static idx_t max_n = 0;
    static bool *prime = NULL;

    /* grow the prime table if needed */
    if (n > max_n) {
        size_t sz = (size_t)(n + 1);
        bool *newprime = (bool*)malloc(sz * sizeof(bool));
        if (!newprime)
            return (seq_t)0; /* allocation failure */

        for (size_t i = 0; i < sz; ++i)
            newprime[i] = true;
        newprime[0] = newprime[1] = false;

        for (idx_t i = 2; i * i <= n; ++i)
            if (newprime[i])
                for (idx_t j = i * i; j <= n; j += i)
                    newprime[j] = false;

        free(prime);
        prime = newprime;
        max_n = n;
    }

    unsigned long long count = 0;

    /* check all bases b with 2 <= b <= n */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t temp = n;
        bool ok = true;
        while (temp) {
            idx_t digit = temp % b;
            if (digit < 2 || (digit <= max_n && !prime[digit])) {
                ok = false;
                break;
            }
            temp /= b;
        }
        if (ok)
            ++count;
    }

    /* representation [n] for all bases > n exists if n is prime */
    if (prime[n])
        ++count;

    return (seq_t)count;
}
