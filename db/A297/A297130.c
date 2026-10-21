/*
 * A297130  Numbers whose base-4 digits d(m), d(m-1),..., d(0) have m=0 or
 *          else d(i) = d(i+1) for some i in {0,1,...,m-1}.
 * https://oeis.org/A297130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297130: numbers whose base‑4 representation contains at least one pair of equal adjacent digits.
   Single‑digit numbers are automatically included.
   The function computes the n‑th term (1‑based index). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 1;   /* next number to test */

    /* ensure cache holds at least n terms */
    while (cache_size < n)
    {
        seq_t x = candidate;
        bool ok;

        if (x < 4)
        {
            ok = true;         /* single digit */
        }
        else
        {
            seq_t prev = x % 4;
            x /= 4;
            ok = false;
            while (x > 0)
            {
                seq_t d = x % 4;
                if (d == prev)
                {
                    ok = true;
                    break;
                }
                prev = d;
                x /= 4;
            }
        }

        if (ok)
        {
            /* grow cache if needed */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }

        candidate++;
    }

    return cache[n - 1];
}
