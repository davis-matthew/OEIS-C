/*
 * A278971  Number of 8-digit numbers whose sum of digits is n.
 * https://oeis.org/A278971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, easy
 * Formula:   a(n) = coefficient of x^n in (x + x^2 + … + x^9)·(1 + x + … +
 *            x^9)^7 This counts 8‑digit decimal numbers with digit sum n
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 8‑digit numbers have digit sums from 1 to 72 (inclusive). */
    if (n < 1 || n > 72)
        return (seq_t)0;

    /* Use a static cache so the table is built only once. */
    static unsigned long long cache[73];
    static bool initialized = false;

    if (!initialized) {
        /* Initialise the table for the first digit (positions 8..2). */
        for (int s = 0; s <= 72; ++s)
            cache[s] = 0ULL;

        /* First (most significant) digit can be 1…9. */
        for (int d = 1; d <= 9; ++d)
            cache[d] = 1ULL;

        /* Add the remaining 7 digits, each 0…9.  For each position we
           compute the convolution with the single‑digit generating
           function (1+x+…+x^9). */
        for (int pos = 2; pos <= 8; ++pos) {
            unsigned long long next[73];
            for (int s = 0; s <= 72; ++s)
                next[s] = 0ULL;

            for (int s = 0; s <= 72; ++s) {
                unsigned long long val = cache[s];
                if (!val)
                    continue;
                for (int d = 0; d <= 9; ++d) {
                    int ns = s + d;
                    if (ns <= 72)
                        next[ns] += val;
                }
            }

            /* Copy back for the next iteration. */
            for (int s = 0; s <= 72; ++s)
                cache[s] = next[s];
        }

        initialized = true;
    }

    return (seq_t)cache[(int)n];
}
