/*
 * A348401  a(n) is the least m > 0 such that n = m! / k! for some k <= m.
 * https://oeis.org/A348401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A348401: a(n) = least m > 0 such that n = m! / k! for some k ≤ m
 *  Computing minimal m by enumerating all products of consecutive integers
 *  up to n = 10000.  The first occurrence of each n gives the minimal m.
 *  Complexity: O(n^2) with n = 10000 → about 50 million multiplications.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* pre‑computed array of answers up to MAX = 10000 */
    static const idx_t MAX = 10000;
    static seq_t *ans = NULL;
    static idx_t size = 0;

    /* Initialise the table once */
    if (!ans)
    {
        size = MAX + 1;                  /* index 0 unused */
        ans = (seq_t *)calloc((size_t)size, sizeof(seq_t));
        if (!ans)
            return 0; /* allocation failure – unlikely in tests */

        for (idx_t m = 1; m <= MAX; ++m)
        {
            uint64_t prod = 1;            /* use 64‑bit for intermediate */
            for (idx_t len = 1; len <= m; ++len)
            {
                prod *= (m - len + 1);    /* multiply by next smaller factor */
                if (prod > MAX)           /* we only care up to MAX */
                    break;
                if (!ans[(size_t)prod])   /* first hit → minimal m */
                    ans[(size_t)prod] = (seq_t)m;
            }
        }
    }

    if (n == 0 || n > MAX)               /* out of pre‑computed range */
        return 0;

    return ans[(size_t)n];
}
