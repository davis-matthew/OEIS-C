/*
 * A284349  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 873", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 256*a(n-8) - 256*a(n-9); Initial values: a(0)=1,
 *            a(1)=0, a(2)=3, ..., a(23)=6291455
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache up to 10000; bump this if the test range grows. */
    static seq_t cache[10001];

    /* The first 24 terms are fixed. */
    static const seq_t base[24] = {
        (seq_t)1,   (seq_t)0,   (seq_t)3,   (seq_t)5,
        (seq_t)10,  (seq_t)29,  (seq_t)58,  (seq_t)95,
        (seq_t)175, (seq_t)503, (seq_t)1019,(seq_t)1525,
        (seq_t)3066,(seq_t)8189,(seq_t)16382,(seq_t)24575,
        (seq_t)45055,(seq_t)129023,(seq_t)262143,(seq_t)393215,
        (seq_t)786431,(seq_t)2097151,(seq_t)4194303,(seq_t)6291455
    };

    /* Return the cached value for the initial block. */
    if (n < 24)
        return base[(size_t)n];

    /* Initialise the cache for the first part of the sequence. */
    for (idx_t i = 0; i < 24; ++i)
        cache[i] = base[i];

    /* Compute further terms using the recurrence. */
    for (idx_t i = 24; i <= n; ++i)
        cache[i] = cache[i - 1]
                 + ((seq_t)256 * cache[i - 8])
                 - ((seq_t)256 * cache[i - 9]);

    return cache[n];
}
