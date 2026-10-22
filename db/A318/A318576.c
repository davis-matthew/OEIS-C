/*
 * A318576  Number of solutions to x^n == 1 (mod n!).
 * https://oeis.org/A318576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p <= n-1} gcd(n,p-1) * p^{v(p,n)} for n odd or n
 *            = 2,4 a(n) = 2 * product_{p <= n-1} gcd(n,p-1) * p^{v(p,n)} for
 *            n even >=6 where v(p,n) is the p-adic valuation of n
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 47, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1) return (seq_t)1;

    /* maintain a global list of primes up to the maximum needed so far */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0; /* largest limit we've built up to */

    /* ensure we have all primes <= n-1 */
    idx_t needed_limit = (n > 0) ? n - 1 : 0;
    if (needed_limit > prime_limit) {
        if (prime_list) free(prime_list);
        build_primes_upto(needed_limit, &prime_list, &prime_cnt);
        prime_limit = needed_limit;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p >= n) break;                    /* only primes <= n-1 */

        /* factor p^v(p,n) */
        uint64_t exp = 0;
        uint64_t temp = n;
        while (temp % p == 0) {
            ++exp;
            temp /= p;
        }

        /* multiply by gcd(n, p-1) */
        seq_t g = gcd((seq_t)n, (seq_t)(p - 1));
        result *= g;

        /* multiply by p^exp (exp may be 0) */
        if (exp > 0) {
            result *= pow_uint(p, (idx_t)exp);
        }
    }

    /* factor 2 for even n >= 6 */
    if (n % 2 == 0 && n >= 6) {
        result *= (seq_t)2;
    }

    return result;
}
