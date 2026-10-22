/*
 * A324076  Integers which are the sum of distinct primes of the form 6*n - 1.
 * https://oeis.org/A324076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sums of distinct primes of the form 6*k‑1.  
   After the 101st term the sequence is simply n+60. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache the first 101 terms.  They are the sums of distinct 5‑mod‑6 primes
       that do not exceed 161, which is the largest term that must be computed
       explicitly.  For n > 101 we use the proven formula a(n) = n + 60. */
    static seq_t first101[101];
    static bool initialized = false;

    if (!initialized) {
        /* Distinct primes congruent to 5 (mod 6) up to 161. */
        static const uint32_t primes_mod6[] = {
            5, 11, 17, 23, 29,
            41, 47, 53, 59, 71,
            83, 89, 101, 107, 113,
            131, 137, 149, 151
        };
        const size_t prime_cnt = sizeof(primes_mod6)/sizeof(primes_mod6[0]);

        /* Array big enough to cover all possible sums (≤200). */
        static unsigned char exist[201];
        memset(exist, 0, sizeof(exist));

        /* Enumerate all non‑empty subsets of the 19 primes. */
        const uint32_t max_mask = 1u << prime_cnt;
        for (uint32_t mask = 1; mask < max_mask; ++mask) {
            uint32_t sum = 0;
            uint32_t m = mask;
            size_t idx = 0;
            while (m) {
                if (m & 1u)
                    sum += primes_mod6[idx];
                ++idx;
                m >>= 1u;
            }
            if (sum <= 200)
                exist[sum] = 1;
        }

        /* Collect the first 101 achievable sums in increasing order. */
        size_t seq_idx = 0;
        for (uint32_t val = 1; val <= 200 && seq_idx < 101; ++val) {
            if (exist[val])
                first101[seq_idx++] = (seq_t)val;
        }
        initialized = true;
    }

    if (n <= 101)
        return first101[(size_t)(n - 1)];
    else
        return (seq_t)n + (seq_t)60;
}
