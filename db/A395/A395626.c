/*
 * A395626  Numbers x*y such that |x-y| is a square, with nonnegative integers
 *          x and y.
 * https://oeis.org/A395626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 5996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395626
   a(n) = the nth smallest non‑negative integer that can be written as
          x*y with |x−y| a perfect square (OEIS workflow).
   a(1) = 0, a(2)=1, a(3)=2, a(4)=4, ...   */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial term 0 explicitly. */
    if (n == 1)
        return (seq_t)0;

    /* We need n-1 positive terms after 0. */
    idx_t need = n - 1;

    /* Initial search bound – a rough over‑estimate. */
    size_t bound = (size_t)(need * 3);
    if (bound < 10) bound = 10;      /* ensure some room for small n */

    while (1)
    {
        /* Memory for numbers up to 'bound'.  Use uint8_t for portable
           C/C++ and cast to bool.  We only need 0/1 values. */
        uint8_t *seen = (uint8_t *)calloc(bound + 1, sizeof(uint8_t));
        if (!seen) return (seq_t)0;   /* allocation failed – impossible here */

        /* --- d = 0 case : squares t*t ----------------------------------- */
        for (size_t t = 1; t <= bound; ++t)
        {
            uint64_t p_int = t * t;
            if (p_int > bound) break;
            seen[(size_t)p_int] = 1;
        }

        /* --- d = k^2, k >= 1 ------------------------------------------- */
        for (size_t k = 1; k * k <= bound; ++k)
        {
            uint64_t d = k * k;
            /* For this d, find all t with t*(t+d) <= bound. */
            for (size_t t = 1; ; ++t)
            {
                uint64_t prod = t * (t + d);
                if (prod > bound) break;
                seen[(size_t)prod] = 1;
            }
        }

        /* Count positive values among seen[1..bound] and find the
           'need'-th one. */
        idx_t cnt = 0;
        seq_t answer = 0;
        for (size_t val = 1; val <= bound; ++val)
        {
            if (seen[val])
            {
                ++cnt;
                if (cnt == need)
                {
                    answer = (seq_t)val;
                    free(seen);
                    return answer;
                }
            }
        }

        /* Not enough terms found – enlarge bound and repeat. */
        free(seen);
        bound *= 2;
    }
}
