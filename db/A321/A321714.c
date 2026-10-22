/*
 * A321714  Numbers k such that lambda(k) = 12.
 * https://oeis.org/A321714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Numbers k such that Carmichael lambda(k) = 12
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by exhaustive enumeration of admissible prime‑power
   combinations.  The set is finite (120 candidates).  The
   implementation precomputes the sorted list once and returns the
   n‑th element on demand. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd_uint64.h"

seq_t a(idx_t n)
{
    /* the list of all numbers with λ(n) = 12, sorted ascending */
    static seq_t values[128];
    static size_t cnt = 0;
    static int initialized = 0;

    if (!initialized) {
        /* enumerated prime‑power exponents (see description) */
        const uint64_t pow2[5]  = {1ULL, 2ULL, 4ULL, 8ULL, 16ULL};
        const uint64_t pow3[3]  = {1ULL, 3ULL, 9ULL};
        const uint64_t pow5[2]  = {1ULL, 5ULL};
        const uint64_t pow7[2]  = {1ULL, 7ULL};
        const uint64_t pow13[2] = {1ULL, 13ULL};

        size_t idx = 0;
        for (int e2 = 0; e2 <= 4; ++e2) {
            uint64_t lam2 = (e2 <= 1) ? 1ULL
                        : ((e2 <= 3) ? 2ULL : 4ULL);
            for (int e3 = 0; e3 <= 2; ++e3) {
                uint64_t lam3 = (e3 == 0) ? 1ULL
                        : ((e3 == 1) ? 2ULL : 6ULL);
                for (int e5 = 0; e5 <= 1; ++e5) {
                    uint64_t lam5 = (e5 == 0) ? 1ULL : 4ULL;
                    for (int e7 = 0; e7 <= 1; ++e7) {
                        uint64_t lam7 = (e7 == 0) ? 1ULL : 6ULL;
                        for (int e13 = 0; e13 <= 1; ++e13) {
                            uint64_t lam13 = (e13 == 0) ? 1ULL : 12ULL;
                            /* lcm of the five λ-values */
                            uint64_t lcm = lam2;
                            uint64_t g   = gcd_uint64(lcm, lam3);
                            lcm = lcm / g * lam3;
                            g   = gcd_uint64(lcm, lam5);
                            lcm = lcm / g * lam5;
                            g   = gcd_uint64(lcm, lam7);
                            lcm = lcm / g * lam7;
                            g   = gcd_uint64(lcm, lam13);
                            lcm = lcm / g * lam13;
                            if (lcm == 12ULL) {
                                uint64_t val = pow2[e2] * pow3[e3]
                                            * pow5[e5] * pow7[e7]
                                            * pow13[e13];
                                values[idx++] = (seq_t)val;
                            }
                        }
                    }
                }
            }
        }
        /* sort the list ascending */
        for (size_t i = 1; i < idx; ++i) {
            seq_t key = values[i];
            size_t j = i;
            while (j > 0 && values[j - 1] > key) {
                values[j] = values[j - 1];
                --j;
            }
            values[j] = key;
        }
        cnt = idx;
        initialized = 1;
    }

    if (n < 1 || (size_t)n > cnt) return 0;
    return values[(size_t)n - 1];
}
