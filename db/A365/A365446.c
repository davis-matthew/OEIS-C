/*
 * A365446  Partial sums of A224613.
 * https://oeis.org/A365446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} sigma(6*k)
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t sigma_of(idx_t m)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            sum += (seq_t)d;
            idx_t q = m / d;
            if (q != d)
                sum += (seq_t)q;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    static seq_t *cum = NULL;
    static size_t cum_sz = 0;
    static idx_t max_n = 0;

    if (cum_sz <= (size_t)n)
    {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cum, new_sz * sizeof(seq_t));
        if (!tmp && new_sz > 0) abort();
        cum = tmp;
        cum_sz = new_sz;
        cum[0] = 0;
    }

    for (idx_t k = max_n + 1; k <= n; ++k)
    {
        idx_t m = 6 * k;
        seq_t s = sigma_of(m);
        cum[(size_t)k] = cum[(size_t)(k - 1)] + s;
    }

    max_n = n;
    return cum[(size_t)n];
}
