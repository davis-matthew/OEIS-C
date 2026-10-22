/*
 * A391120  a(n) is the least k for which n^k contains at least one decimal
 *          digit 9, or -1 if there is no such k.
 * https://oeis.org/A391120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = least k for which n^k contains decimal digit 9; return -1
 *            if no such k. Property a(10*k)=a(k) is exploited by dropping
 *            trailing zeros from n
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 31, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* strip trailing zeros: a(10*k)=a(k) */
    while ((n % (idx_t)10) == 0)
        n /= 10;

    seq_t base = (seq_t)n;          /* store base as seq_t for arithmetic */
    unsigned int freq[10];

    /* check if n itself already contains a 9 */
    digit_frequencies(base, freq);
    if (freq[9])
        return (seq_t)1;

    seq_t val = base;               /* current power: base^k, starts at k=1 */
    const int MAX_K = 20;           /* safe upper bound; conjecture says <=12 */

    for (int k = 2; k <= MAX_K; ++k) {
        val *= base;                /* advance to next power */
        digit_frequencies(val, freq);
        if (freq[9])
            return (seq_t)k;
    }

    return (seq_t)-1;               /* no 9 digit found within bound */
}
