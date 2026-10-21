/*
 * A248334  The subsequence of A246885 having even values.
 * https://oeis.org/A248334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest even integer that can be written an
 *            odd number of times as 2·r³ + 4·s³ with r,s ≥ 0. The function
 *            enumerates all such representations up to a bound, records the
 *            parity of each sum, and selects the n‑th odd‑count value
 * Range:     u8 n <= 22, u16 n <= 720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, but guard anyway */

    /* estimate a bound; grow until enough odd values are found */
    idx_t limit = n > 10 ? n * 10 : 100;   /* start reasonably large */

    while (1) {
        /* allocate and zero parity table */
        uint8_t *parity = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
        if (!parity) return (seq_t)0;     /* allocation failure fallback */

        /* toggle parity for all sums 2r³+4s³ <= limit */
        for (idx_t r = 0;; ++r) {
            unsigned long long r3 = (unsigned long long)r * r * r;
            unsigned long long two_r3 = 2ULL * r3;
            if (two_r3 > (unsigned long long)limit) break;

            for (idx_t s = 0;; ++s) {
                unsigned long long s3 = (unsigned long long)s * s * s;
                unsigned long long sum = two_r3 + 4ULL * s3;
                if (sum > (unsigned long long)limit) break;

                parity[(idx_t)sum] ^= 1;
            }
        }

        /* scan the table to find the n-th odd‑count value */
        idx_t count = 0;
        idx_t result = 0; /* will hold the n-th value */

        for (idx_t i = 0; i <= limit; ++i) {
            if (parity[i] & 1) {
                ++count;
                if (count == n) {
                    result = i;
                    break;  /* found */
                }
            }
        }

        free(parity);

        if (count >= n)
            return (seq_t)result;      /* success */

        /* not enough, double the limit and try again */
        limit <<= 1;
    }
}
