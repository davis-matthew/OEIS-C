/*
 * A275956  Numbers n for which A060502(n) = A275806(n); numbers whose
 *          factorial base representation has an equal number of distinct
 *          nonzero digits and occupied slopes.
 * https://oeis.org/A275956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 8910, u32 n <= 8910, u64 n <= 8910,
 *            u128 n <= 8910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275956: numbers where the number of distinct nonzero digits in the
   factorial base representation equals the number of distinct values of (i - d)
   for each nonzero digit d at position i (starting from 1).  Offset: 0 */
#include <stdbool.h>

/* Test whether n satisfies the condition for A275956 */
static bool qualifies(seq_t n)
{
    if (n == 0)          /* 0 is explicitly included in the sequence */
        return true;

    /* MAX_DIGITS should be large enough for all numbers we will test.
       For 64‑bit numbers the factorial base needs at most 20 digits,
       but we allocate 512 just to be safe for arbitrary‑precision values. */
    bool seen_digit[512] = {0};
    bool seen_diff[512]  = {0};
    unsigned int max_i  = 0;

    seq_t temp = n;
    unsigned int i = 1;                /* position (coefficient of i!) */
    while (temp > 0) {
        seq_t denom = (seq_t)(i + 1);   /* base for this digit */
        seq_t d = temp % denom;        /* digit at position i */
        temp /= denom;                 /* remainder for next position */

        if (d != 0) {
            unsigned int di = (unsigned int)d;
            seen_digit[di] = true;      /* distinct nonzero digit */
            unsigned int diff = i - di;
            seen_diff[diff] = true;     /* distinct (i - d) value */
        }
        if (i > max_i) max_i = i;
        ++i;
    }

    /* Count distinct nonzero digits */
    unsigned int distinct_digits = 0;
    for (unsigned int k = 1; k <= max_i; ++k)
        if (seen_digit[k]) ++distinct_digits;

    /* Count distinct (i - d) values */
    unsigned int distinct_diffs = 0;
    /* diff can reach up to max_i-1, but we scan to max_i to be safe */
    for (unsigned int k = 0; k <= max_i; ++k)
        if (seen_diff[k]) ++distinct_diffs;

    return distinct_digits == distinct_diffs;
}

/* Return the nth element of A275956 (0‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* 0 is the first term */

    seq_t candidate = 0;
    idx_t count = 0;        /* number of qualifying numbers found */

    while (1) {
        if (qualifies(candidate)) {
            if (count == n)
                return candidate;
            ++count;
        }
        ++candidate;
    }
}
