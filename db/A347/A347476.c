/*
 * A347476  Numbers which give a prime number when 0's and 1's are
 *          interchanged in their binary representation.
 * https://oeis.org/A347476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates terms of A347476 by iterating over binary lengths
   and the primes that fit in the lower half of that length. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n==0, return 0 */
        return 0;

    /* Once per process: build the list of primes up to 65535. */
    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;
    if (primes == NULL) {
        /* build_primes_upto allocates memory that we keep for the lifetime
           of the process; no need to free. */
        build_primes_upto((idx_t)65535, &primes, &prime_cnt);
    }

    idx_t count = 0;

    /* The largest limit we need for primes is 2^(len-1)-1.
       For n ≤ 10 000, len never exceeds 17, so 65535 is ample. */
    const idx_t PRIME_LIMIT = 65535;

    for (idx_t len = 2; ; ++len) {
        /* Upper prime bound for this length. */
        idx_t p_max = ((idx_t)1 << (len - 1)) - 1;
        if (p_max > PRIME_LIMIT)
            p_max = PRIME_LIMIT;

        /* Find how many primes are ≤ p_max. */
        size_t up = 0;
        while (up < prime_cnt && primes[up] <= (uint64_t)p_max)
            ++up;

        /* Enumerate those primes in *decreasing* order to produce
           ascending x (the term values). */
        for (size_t pi = up; pi-- > 0; ) {
            uint64_t prime = primes[pi];

            /* Compute x = 2^len - 1 - prime using 64‑bit arithmetic
               to avoid overflow on small seq_t types. */
            uint64_t x_u64 = ((uint64_t)1 << len) - 1 - prime;

            /* If seq_t is 64‑bit or larger, the cast is safe. */
            /* If seq_t is smaller, ensure we don't wrap. */
            seq_t result;
            if (sizeof(seq_t) <= sizeof(uint64_t)) {
                seq_t tmp = (seq_t)x_u64;
                if ((uint64_t)tmp != x_u64)
                    continue;          /* value would overflow seq_t */
                result = tmp;
            } else {
                result = (seq_t)x_u64;
            }

            ++count;
            if (count == n)
                return result;
        }
    }
}
