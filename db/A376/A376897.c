/*
 * A376897  Positive numbers k such that all the digits in the octal expansion
 *          of k^2 are distinct.
 * https://oeis.org/A376897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   numbers k such that all digits of k^2 in base 8 are distinct.
 *            The function computes the n-th term by precomputing all such k
 *            up to the theoretical maximum (k < 2^12 = 4096), which suffices
 *            for any n
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t terms[5000];
    static idx_t term_count = 0;
    static bool initialized = false;

    if (!initialized) {
        const idx_t MAX_K = (1ULL << 12) - 1;   /* 4095 */
        for (idx_t k = 1; k <= MAX_K; ++k) {
            seq_t sq = (seq_t)k * (seq_t)k;

            bool distinct = true;
            seq_t tmp = sq;
            unsigned int mask = 0;

            do {
                unsigned int digit = (unsigned int)(tmp % (seq_t)8);
                if (mask & (1U << digit)) {        /* duplicate digit */
                    distinct = false;
                    break;
                }
                mask |= (1U << digit);
                tmp /= (seq_t)8;
            } while (tmp > (seq_t)0);

            if (distinct) {
                terms[term_count++] = (seq_t)k;
            }
        }
        initialized = true;
    }

    /* Assumes n >= 1 and within precomputed bounds */
    return terms[n - 1];
}
