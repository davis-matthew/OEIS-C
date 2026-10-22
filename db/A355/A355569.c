/*
 * A355569  Numbers k > 4 in a Collatz trajectory reaching k after starting at
 *          k+1.
 * https://oeis.org/A355569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th integer k > 4 such that the Collatz
 * trajectory starting at k+1 reaches k.
 * Brute‑force search with memoization. */
#include <stdbool.h>
#include <stdlib.h>

/* Note: realloc returns void* in C; cast it for C++ compatibility. */
seq_t a(idx_t n)
{
    /* static cached terms and state */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    /* first candidate k to test */
    static seq_t next_candidate = 5;

    /* generate terms until we have at least n */
    while (cache_len < n)
    {
        seq_t k = next_candidate;   /* candidate k (>4) */
        seq_t x = k + 1;            /* start of Collatz orbit */
        const idx_t MAX_STEPS = 50000;
        idx_t step = 0;
        bool found = false;

        while (step < MAX_STEPS && !found)
        {
            if (x == k) { found = true; break; }
            if (x == 1) break;

            /* standard Collatz step */
            if ((x & 1) == 0)   /* x even */
                x = x / 2;
            else                /* x odd */
                x = 3 * x + 1;
            ++step;
        }

        if (found)   /* k satisfies the condition */
        {
            if (cache_len == cache_cap)          /* grow cache */
            {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                /* cast realloc return for C++ */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }

        /* advance to next candidate */
        next_candidate = k + 1;
    }

    return cache[n - 1];
}
