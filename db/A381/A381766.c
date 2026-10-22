/*
 * A381766  Starting from the n-th prime, a(n) is the minimum number > 1 of
 *          consecutive primes whose sum is the average of a twin prime pair.
 * https://oeis.org/A381766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal m>1 such that the sum of the m consecutive primes
 *            * starting from the n-th prime equals the average (p+1) of a
 *            twin prime pair (p,p+2)
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* return true iff val is even and both val-1 and val+1 are prime     */
static bool is_twin_avg(seq_t val)
{
    /* val must be even; otherwise it cannot be an average of a twin pair */
    if ((val & 1) == 0) {
        seq_t low  = val - 1;
        seq_t high = val + 1;
        return is_prime_big(low) && is_prime_big(high);
    }
    return false;
}

seq_t a(idx_t n)
{
    /* sum of primes starting from the n-th prime */
    seq_t sum = (seq_t)get_prime(n);
    idx_t m   = 1; /* number of primes already summed */

    for (;;) {
        /* next prime index is n+m (current m counts primes already summed) */
        idx_t next_idx = n + m;
        sum += (seq_t)get_prime(next_idx);
        ++m;

        /* check only after at least two primes have been added */
        if (m > 1 && is_twin_avg(sum))
            return m;
    }
}
