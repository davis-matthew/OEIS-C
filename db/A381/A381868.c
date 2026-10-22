/*
 * A381868  Starting from the n-th prime, a(n) is the minimum number > 1 of
 *          consecutive primes whose sum is the greater of a twin prime pair.
 * https://oeis.org/A381868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the minimum k > 1 such that the sum of the k consecutive
 *            primes starting at the n-th prime is a twin prime (i.e., a prime
 *            q with q-2 also prime)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

static bool is_twin_prime(seq_t x) {
    if (x < 5) return false;          /* smallest twin prime is 5 (3,5) */
    return is_prime_big(x) && is_prime_big(x - (seq_t)2);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined for offset < 1, return 0 */

    seq_t sum = 0;                    /* running sum of primes */
    idx_t i = n;                      /* current prime index */
    bool found = false;

    for (;;) {
        sum += (seq_t)get_prime(i);   /* add current prime to sum */
        if (i - n + 1 > (idx_t)1) {   /* number of terms > 1 */
            if (is_twin_prime(sum)) {
                found = true;
                break;
            }
        }
        ++i;                          /* move to next prime */
    }

    return found ? (seq_t)(i - n + 1) : (seq_t)0;
}
