/*
 * A322177  If n = Product (p_j^k_j) then a(n) = Sum (prime(p_j)^prime(k_j)).
 * https://oeis.org/A322177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p^k||n} prime(p)^prime(k)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 9, etc.  (see OEIS A322177) */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0 */
        return 0;

    seq_t sum = 0;
    idx_t temp = n;

    /* trial division for small primes */
    for (idx_t p = 2; (unsigned __int128)p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp_cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp_cnt;
            }
            seq_t base = (seq_t)get_prime(p);      /* prime(p)   */
            seq_t exp  = (seq_t)get_prime(exp_cnt);/* prime(k)   */
            sum += big_pow(base, exp);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t base = (seq_t)get_prime(temp);         /* prime(p)   */
        seq_t exp  = (seq_t)get_prime(1);            /* prime(1)=2 */
        sum += big_pow(base, exp);
    }

    return sum;
}
