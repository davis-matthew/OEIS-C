/*
 * A274944  Numbers with at least three digits and with the property that the
 *          sum of the squares of the first and last digits equals the number
 *          obtained when the first and last digits are deleted.
 * https://oeis.org/A274944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   A274944 – numbers n = d1 d2 … dk (k≥3, d2≠0) such that d1^2 +
 *            dk^2 = d2…dk-1
 * Range:     u8 n <= 5, u16 n <= 79, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is finite; the largest term is 91629.  All terms are
 * 3‑5 digits long.  The function lazily constructs the list of
 * admissible numbers once and then returns the requested element. */

#include <stddef.h>

/* Helper: compute the number of decimal digits of a positive integer. */
static int digit_len(int x)
{
    int len = 0;
    do
    {
        ++len;
        x /= 10;
    } while (x > 0);
    return len;
}

seq_t a(idx_t n)
{
    /* Static cache of terms – a maximum of 100 000 entries is more than
     * enough for this finite sequence.  The array is filled lazily on the
     * first call. */
    static seq_t cache[100000];
    static size_t cache_len = 0;

    if (cache_len == 0)          /* first invocation: generate the cache */
    {
        static const int pow10[6] = {1, 10, 100, 1000, 10000, 100000};

        for (int num = 100; num <= 99999; ++num)
        {
            int first   = num;
            while (first >= 10) first /= 10;

            int last    = num % 10;
            int len     = digit_len(num);

            int pow10lenminus2 = pow10[len - 2];          /* 10^(len-2) */

            int second  = (num / pow10lenminus2) % 10;    /* d2 */
            if (second == 0) continue;                    /* skip if d2 = 0 */

            int middle  = (num / 10) % pow10lenminus2;    /* d2…dk-1 */

            int sumsq   = first * first + last * last;    /* d1^2 + dk^2 */

            if (sumsq == middle)
                cache[cache_len++] = (seq_t)num;
        }
    }

    /* The OEIS b-file contains only the terms that exist; the
     * caller is expected to provide n within that range.  No bounds check. */
    return cache[n - 1];
}
