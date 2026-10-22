/*
 * A342156  For n > 2, a(n) = 0,1,2, or 3 when (prime(n+1) mod 6, prime(n) mod
 *          6) = (1,1),(1,5),(5,1), or (5,5), respectively.
 * https://oeis.org/A342156
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0,1,2,3 when (prime(n+1) mod 6, prime(n) mod 6) =
 *            (1,1),(1,5),(5,1),(5,5) respectively
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t used_limit = 0;

    if (primes_cnt <= n) {   /* need at least n+1 primes (prime indexing starts at 1) */
        if (primes) {
            free(primes);
            primes = NULL;
        }
        used_limit = n * 20 + 100;
        if (used_limit < 200) used_limit = 200;
        while (1) {
            size_t count = 0;
            uint64_t *tmp = NULL;
            build_primes_upto(used_limit, &tmp, &count);
            if (count >= n + 1) {
                primes = tmp;
                primes_cnt = count;
                break;
            }
            free(tmp);
            used_limit *= 2;
        }
    }

    uint64_t p = primes[n - 1]; /* prime(n)  */
    uint64_t q = primes[n];     /* prime(n+1) */

    uint64_t modp = p % 6;
    uint64_t modq = q % 6;

    seq_t res;
    if (modq == 1 && modp == 1) {
        res = 0;
    } else if (modq == 1 && modp == 5) {
        res = 1;
    } else if (modq == 5 && modp == 1) {
        res = 2;
    } else if (modq == 5 && modp == 5) {
        res = 3;
    } else {
        res = 0; /* never reached for n >= 3 */
    }
    return res;
}
