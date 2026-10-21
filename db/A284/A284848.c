/*
 * A284848  {1100->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A284848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transformation: replace every occurrence of 1100 in the Thue–Morse
 * word by a single 1, and output the resulting binary word as a sequence
 * a(n).  The implementation scans the Thue–Morse bits with a look‑ahead
 * window, buffers generated terms, and memoises them so calls with
 * arbitrary n are answered in O(n). */

#include <stdlib.h>
#include <stdbool.h>

static inline seq_t tm_bit(idx_t pos)
{
    /* Thue–Morse bit t(pos) = parity(popcount(pos)) for pos = 0,1,2,... */
#if defined(__GNUC__) || defined(__clang__)
    return (seq_t)__builtin_parityll(pos);
#else
    unsigned int p = 0;
    idx_t v = pos;
    while (v)
    {
        p ^= (v & 1ULL);
        v >>= 1;
    }
    return (seq_t)p;
#endif
}

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Position in the Thue–Morse word from which the next bit is read
     * (0‑based).  This value is persistent between calls. */
    static idx_t next_input = 0;

    /* Grow the cache until we have at least n terms. */
    if (n > cache_size)
    {
        /* Ensure the array has sufficient capacity. */
        if (!cache)
        {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache)
                return (seq_t)0; /* allocation failure */
        }
        while (cache_size < n)
        {
            /* Fetch the next four bits to test the pattern 1100. */
            seq_t b0 = tm_bit(next_input);
            seq_t b1 = tm_bit(next_input + 1);
            seq_t b2 = tm_bit(next_input + 2);
            seq_t b3 = tm_bit(next_input + 3);

            seq_t out_bit;
            if (b0 == 1 && b1 == 1 && b2 == 0 && b3 == 0)
            {
                out_bit = (seq_t)1;       /* pattern 1100 → single 1 */
                next_input += 4;
            }
            else
            {
                out_bit = b0;             /* otherwise keep the current bit */
                next_input += 1;
            }

            /* Resize the cache if needed. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;       /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_size++] = out_bit;
        }
    }

    return cache[n - 1];
}
