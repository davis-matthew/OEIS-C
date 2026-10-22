/*
 * A376898  Positive numbers k such that all the digits in the octal expansion
 *          of k^3 are distinct.
 * https://oeis.org/A376898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = nth positive integer k such that all digits in the octal
 *            expansion of k^3 are distinct
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t terms[256];
    static size_t term_count = 0;

    if (!initialized) {
        /* Precompute all qualifying k < 256 (since k^3 < 2^24). */
        for (idx_t k = 1; k < 256; ++k) {
            unsigned long long cube = (unsigned long long)k * k * k;
            unsigned mask = 0;
            unsigned long long t = cube;
            bool ok = true;
            while (t) {
                unsigned d = t & 7U;          /* t % 8 */
                if (mask & (1U << d)) {       /* digit repeats */
                    ok = false;
                    break;
                }
                mask |= 1U << d;
                t >>= 3;                      /* t /= 8 */
            }
            if (ok) {
                terms[term_count++] = (seq_t)k;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > term_count) {
        return 0;                          /* undefined for out-of-range n */
    }
    return terms[n - 1];
}
