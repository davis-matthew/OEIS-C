/*
 * A363896  Numbers k such that the sum of primes dividing k (with repetition)
 *          is equal to Euler's totient function of k.
 * https://oeis.org/A363896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363896: numbers k such that the sum of the primes (with multiplicity) dividing k
   equals Euler’s totient function of k. The sequence is finite with terms 9, 15, 16, 42. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* upper bound chosen because no further terms are known below 1.6×10⁷ */
    const idx_t LIMIT = 16000000ull;
    if (n < 1) return 0;

    idx_t found = 0;
    for (idx_t k = 2; k <= LIMIT; ++k) {
        uint64_t x = k;
        seq_t sum_pf = 0;          /* sum of prime factors with multiplicity */
        seq_t phi    = k;          /* Euler's totient of k */

        /* factorization, handling each prime once for phi update */
        for (uint64_t p = 2; p * p <= x; ++p) {
            if (x % p == 0) {
                uint64_t exp = 0;
                while (x % p == 0) {
                    x /= p;
                    exp++;
                }
                sum_pf += (seq_t)p * (seq_t)exp;
                phi    -= phi / (seq_t)p;
            }
        }
        if (x > 1) {                /* remaining prime factor */
            sum_pf += (seq_t)x;
            phi    -= phi / (seq_t)x;
        }

        if (sum_pf == phi) {
            found++;
            if (found == n) {
                return (seq_t)k;
            }
        }
    }
    return 0;   /* no further terms exist */
}
