/*
 * A260214  a(n) is the first triangular number whose decimal representation
 *          uses n distinct digits.
 * https://oeis.org/A260214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n*(n+1)/2 (here a(n) = first triangular number whose
 *            decimal representation uses n distinct digits)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_frequencies.h"

/* Count how many distinct decimal digits are present in a seq_t value. */
static unsigned count_distinct_digits(seq_t v)
{
    unsigned freq[10];
    digit_frequencies(v, freq);
    unsigned cnt = 0;
    for (int d = 0; d < 10; ++d)
        if (freq[d])
            ++cnt;
    return cnt;
}

/* a(n): first triangular number with n distinct decimal digits.
   For n > 10 the definition is meaningless, so return 0. */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1)
        return 0;
    if (n > 10)
        return 0;                      /* no such number with >10 distinct digits */

    static seq_t cache[11] = {0};
    static bool done[11]  = {false};

    if (done[n])
        return cache[n];

    seq_t t = 0;
    for (idx_t k = 1; ; ++k) {
        /* increment the triangular number */
        t += (seq_t)k;
        if (count_distinct_digits(t) == n) {
            cache[n] = t;
            done[n]  = true;
            return t;
        }
    }
}
