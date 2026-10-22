/*
 * A324656  a(n) is the number of successive primorials A002110(i) larger than
 *          n that need to be tried before sum n + A002110(i) is found to be
 *          composite.
 * https://oeis.org/A324656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 9345, u128 n <= 9345,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A324656
 *
 *  a(n) = the number of successive primorials P_i > n such that
 *        n + P_i is prime, until a composite sum n + P_i is found.
 *
 *  Implementation details:
 *      - The first primorial larger than n is constructed by
 *        repeated multiplication by successive primes obtained via
 *        get_prime(k).
 *      - For each primorial the sum with n is tested for primality
 *        with is_prime_big (works for both integer and big-integer
 *        builds).
 *      - No memoisation or static storage is used; each call builds
 *        primorials on the fly.  The algorithm is O(k) where k is
 *        the number of primorials needed before a composite appears,
 *        which is very small for all n examined (≤10000).
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* cast index to seq_t for arithmetic */

    /* Build the first primorial strictly larger than n */
    seq_t primorial = 2;            /* 2# */
    idx_t prime_index = 1;          /* get_prime(1) == 2 */

    while (primorial <= nn) {
        prime_index++;             /* next prime index */
        primorial = primorial *
                    (seq_t)get_prime(prime_index); /* multiply by next prime */
    }

    /* Count how many successive primorials give a prime sum */
    seq_t count = 0;
    while (true) {
        seq_t sum = primorial + nn;
        if (!is_prime_big(sum))
            break;                 /* composite sum encountered */
        count++;                  /* prime sum found      */
        /* Advance to next primorial */
        prime_index++;
        primorial = primorial *
                    (seq_t)get_prime(prime_index);
    }

    return count;
}
