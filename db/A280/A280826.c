/*
 * A280826  Numbers with an even number of digits and with an odd number of
 *          distinct digits.
 * https://oeis.org/A280826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose decimal representation has *
 *            an even number of digits and an odd number of distinct digits
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_counts, digit_len, seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/digit_counts.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* The sequence is indexed starting at 1.  Return 0 for n == 0. */
    if (n == 0)
        return 0;

    /* static cache of previously computed terms */
    static seq_t *cache      = NULL;   /* array of seq_t */
    static idx_t cache_size = 0;       /* number of terms currently stored   */
    static idx_t cache_cap  = 0;       /* current allocation size (in terms) */
    static seq_t next_candidate = 10;  /* next value to test (decimal 10)     */

    /* Ensure the cache can hold at least n terms */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 128;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *new_buf = seq_alloc(new_cap);
        for (idx_t i = 0; i < cache_size; ++i)
            new_buf[i] = cache[i];
        if (cache != NULL)
            seq_free(cache);
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* Generate terms until we have the n-th term */
    while (cache_size < n) {
        /* Test the current candidate */
        int len = digit_len(next_candidate);
        if ((len & 1) == 0) { /* even length? */
            unsigned char cnt[10] = {0};
            digit_counts(next_candidate, cnt);
            int distinct = 0;
            for (int d = 0; d < 10; ++d)
                if (cnt[d] > 0)
                    ++distinct;
            if ((distinct & 1) == 1) {
                /* Candidate satisfies the condition.  Store it. */
                cache[cache_size++] = next_candidate;
            }
        }
        /* Advance to the next integer to test */
        next_candidate++;
    }

    return cache[n - 1];
}
