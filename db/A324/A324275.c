/*
 * A324275  Numbers k for which A324274(k) is 0, i.e., starting squares in
 *          A324274 that yield a path of infinite length.
 * https://oeis.org/A324275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5) - a(n-6) +
 *            a(n-7) for n>7; a(1) = 2, a(2) = 9, a(3) = 14, a(4) = 27, a(5) =
 *            34, a(6) = 53, a(7) = 64
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[7] = {2, 9, 14, 27, 34, 53, 64};
static seq_t *memo = NULL;
static size_t memo_size = 0;
static size_t computed = 0;

seq_t a(idx_t n)
{
    if (n <= 7)
        return init[n - 1];

    size_t nn = (size_t)n;

    /* ensure storage for all indices up to nn */
    if (memo == NULL || memo_size <= nn)
    {
        size_t newsize = nn + 1;                /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(memo, newsize * sizeof(seq_t));
        if (tmp == NULL)
            abort();                            /* out of memory */
        memo = tmp;
        memo_size = newsize;
    }

    /* initialise the first seven entries if not already done */
    if (computed == 0)
    {
        for (size_t i = 1; i <= 7; ++i)
            memo[i] = init[i - 1];
        computed = 7;
    }

    /* compute missing values up to nn */
    if (nn > computed)
    {
        for (size_t i = computed + 1; i <= nn; ++i)
        {
            memo[i] = memo[i - 1] + memo[i - 2] - memo[i - 3] + memo[i - 4]
                      - memo[i - 5] - memo[i - 6] + memo[i - 7];
        }
        computed = nn;
    }

    return memo[nn];
}
