/*
 * A360311  The sum of the primes prime(n) + prime(n+1) + ... + prime(n+k) in
 *          A360297.
 * https://oeis.org/A360311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = sum of primes p_n, p_{n+1}, …, p_{n+k} where k is the
 *            smallest integer ≥0 such that the sum is divisible by the next
 *            prime p_{n+k+1}
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 11, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* initialize prime table once */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static bool initialized = false;

    if (!initialized) {
        /* limit chosen large enough to cover all indices needed for n ≤ 10000
           (≈2.4 M primes up to 20 M). */
        const size_t limit = 20000000;
        build_primes_upto(limit, &primes, &primes_cnt);
        initialized = true;
    }

    if (n == 0) return 0;

    /* Convert the 1‑based index n to a 0‑based array index. */
    size_t start = (size_t)n - 1;
    seq_t sum = 0;

    /* Iterate over primes starting at p_n until the sum is divisible by
       the following prime. */
    for (size_t m = start; m + 1 < primes_cnt; ++m) {
        sum += (seq_t)primes[m];
        uint64_t next = primes[m + 1];
        if ((sum % (seq_t)next) == 0) {
            return sum;
        }
    }

    /* The chosen limit should be sufficient; if we reach this point,
       just return the accumulated sum. */
    return sum;
}
