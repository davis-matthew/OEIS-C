/*
 * A348548  Positive integers that are the product of two integers ending with
 *          8.
 * https://oeis.org/A348548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A348548: numbers that are the product of two integers ending with 8.
 *     a(n) = nth smallest positive integer t such that t = x*y,
 *            with x ≡ 8 (mod 10) and y ≡ 8 (mod 10).
 * This implementation builds all such products up to a dynamically
 * increased bound and returns the n‑th distinct value.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* minimal bound to start; will grow if needed */
    size_t L = (size_t)n * 25 + 100;
    if (L < 64) L = 64;

    /* static buffer reused across calls */
    static unsigned char *present = NULL;
    static size_t present_size = 0;

    seq_t result = 0;

    while (1)
    {
        /* allocate / grow presence array */
        if (present_size < L + 1)
        {
            free(present);
            present = (unsigned char *)malloc(L + 1);
            present_size = L + 1;
            if (!present) return 0; /* allocation failure, return 0 */
        }

        /* clear presence marks */
        memset(present, 0, present_size);

        /* generate all products u*v <= L with u,v ending in 8 */
        for (idx_t u = 8; u <= (idx_t)L; u += 10)
        {
            for (idx_t v = u; v <= (idx_t)L; v += 10)
            {
                seq_t prod = (seq_t)u * (seq_t)v;
                if (prod > (seq_t)L) break;
                present[(size_t)prod] = 1;
            }
        }

        /* search for the n‑th distinct product */
        idx_t count = 0;
        for (size_t val = 1; val <= L; ++val)
        {
            if (present[val])
            {
                ++count;
                if (count == n)
                {
                    result = (seq_t)val;
                    return result;
                }
            }
        }

        /* not enough terms yet – increase bound and retry */
        L *= 2;
    }
}
