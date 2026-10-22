/*
 * A384046  Triangle in which the n-th row gives the numbers from 1 to n whose
 *          largest divisor that is a unitary divisor of n is 1.
 * https://oeis.org/A384046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) enumerates, row by row, the integers k (1≤k≤r) such that
 *            for every prime power p^e dividing r, p^e does NOT divide k. The
 *            sequence a(n) is the nth element of this flattened triangle
 * Range:     u8 n <= 9323, u16 n <= 9323, u32 n <= 9323, u64 n <= 9323,
 *            u128 n <= 9323, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Be sure to include <stddef.h> so that size_t is available. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t cumulative = 0;          /* number of elements already seen */

    for (idx_t r = 1; ; ++r) {
        /* --- factor r into prime powers (unitary divisors) --- */
        idx_t tmp = r;
        /* store the prime powers in this small array (r has at most 64 primes). */
        idx_t powers[64];
        size_t pc = 0;

        for (idx_t p = 2; p * p <= tmp; ) {
            if (tmp % p == 0) {
                idx_t pw = 1;
                do {
                    tmp /= p;
                    pw *= p;
                } while (tmp % p == 0);
                powers[pc++] = pw;
                if (p == 2) p = 3; else p += 2;
            } else {
                if (p == 2) p = 3; else p += 2;
            }
        }
        if (tmp > 1) {          /* remaining prime factor */
            powers[pc++] = tmp;
        }

        /* --- enumerate valid numbers in this row --- */
        idx_t local_count = 0;

        for (idx_t k = 1; k <= r; ++k) {
            /* check if k is divisible by any of the prime powers */
            int bad = 0;
            for (size_t i = 0; i < pc; ++i) {
                if (k % powers[i] == 0) { bad = 1; break; }
            }
            if (!bad) {
                ++local_count;     /* new valid element discovered */

                if (cumulative + local_count == n) {
                    return (seq_t)k;
                }
            }
        }

        cumulative += local_count;
        /* If cumulative >= n, the answer has already been returned above. */
    }
}
