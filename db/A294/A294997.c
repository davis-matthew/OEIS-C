/*
 * A294997  Numbers k such that the largest digit of k^3 is 7.
 * https://oeis.org/A294997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 216, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   big_pow, digit_counts, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A294997 – numbers k such that the largest digit of k³ is 7 */
/* Computation is performed by brute‑force enumeration, memoised in a static array. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/big_pow.h"          /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/digit_counts.h"     /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/seq_realloc.h"      /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); */

/* Return the maximum decimal digit of k³ */
static unsigned max_digit_in_cube(idx_t k)
{
    seq_t cube = big_pow((seq_t)k, (seq_t)3);
    unsigned char cnt[10] = {0};
    digit_counts(cube, cnt);
    for (int d = 9; d >= 0; --d)
        if (cnt[d])
            return (unsigned)d;
    return 0; /* never reached for k > 0 */
}

/* Public entry */
seq_t a(idx_t n)
{
    /* lazy, static, order‑independent memoisation */
    static seq_t *cache     = NULL; /* array of found terms         */
    static size_t cache_cap = 0;    /* allocated capacity           */
    static size_t cache_len = 0;    /* number of terms stored       */
    static idx_t  next      = 1;    /* next integer to test         */

    while (cache_len < n)
    {
        if (max_digit_in_cube(next) == 7)   /* satisfies the condition */
        {
            /* grow cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next;
        }
        ++next;
    }
    return cache[n - 1];
}
