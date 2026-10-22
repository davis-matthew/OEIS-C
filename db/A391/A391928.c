/*
 * A391928  Irregular table T(n, k) = (k*A002110(n-1)) mod prime(n), n >= 0, k
 *          = 0..prime(n)-1, read by rows, with row 0 = {0} by convention.
 * https://oeis.org/A391928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) = (k*A062347(n-1)) mod prime(n) where k = a(n) position in
 *            its row * A062347(n) is the primorial of the n-th prime (product
 *            of all primes ≤ prime(n)). * The sequence is read row‑by‑row,
 *            with row 0 = {0} by convention
 * Range:     u8 n <= 6200, u16 n <= 8855, u32 n <= 8855, u64 n <= 8855,
 *            u128 n <= 8855, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* find the row that contains index n and the prime of that row */
    idx_t cum = 1;           /* cumulative count of terms up to the start of next row */
    idx_t row = 0;
    uint32_t row_prime = 0;

    for (idx_t r = 1; ; ++r)
    {
        uint32_t pr = get_prime(r);
        if (cum + pr > n)
        {
            row = r;
            row_prime = pr;
            break;
        }
        cum += pr;
    }

    /* position of n within its row (0‑based) */
    idx_t k = n - cum;               /* we keep k as idx_t to avoid overflow */
    seq_t k_seq = (seq_t)k;

    /* compute A062347(row-1) modulo row_prime:
     * product of primes 2,3,5,...,prime(row-1) modulo row_prime
     */
    seq_t factor = 1;
    uint32_t mod_prime = (uint32_t)row_prime;
    for (idx_t i = 1; i < row; ++i)   /* primes up to prime(row-1) */
    {
        seq_t pi = (seq_t)get_prime(i);
        factor = (factor * pi) % (seq_t)mod_prime;
    }

    /* final value */
    seq_t result = (k_seq * factor) % (seq_t)mod_prime;
    return result;
}
