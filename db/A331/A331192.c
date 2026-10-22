/*
 * A331192  Numbers whose Zeckendorf representation (A014417) and dual
 *          Zeckendorf representation (A104326) are both palindromic.
 * https://oeis.org/A331192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th smallest number of the form F(2k-1)-1 (k>0) or
 *            2*F(2k-1)-4 (k>1), where F(m) is the m‑th Fibonacci number with
 *            F(1)=1, F(2)=1. The two sequences are increasing and interleave,
 *            so we generate them in order, skip the duplicate 0, and cache
 *            the results for further calls
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of generated terms (ascending order) */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;

    /* static state for Fibonacci generation */
    static seq_t f_prev = 0;   /* F(0) */
    static seq_t f_curr = 1;   /* F(1) */
    static idx_t  fib_idx = 1; /* current index of f_curr */

    /* static state for candidate generation */
    static idx_t next_k = 1;   /* next k to process */
    static seq_t last_val = 0;
    static bool   last_valid = false;

    /* generate terms until we have at least n of them */
    while (len < n)
    {
        /* compute Fibonacci number F(2*next_k-1) */
        idx_t target = 2 * next_k - 1;
        while (fib_idx < target)
        {
            seq_t temp = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = temp;
            fib_idx++;
        }
        /* now f_curr == F(2*next_k-1) */
        seq_t x = f_curr - 1;   /* candidate F(2k-1)-1 */

        /* add x if it is larger than the last stored value */
        if (!last_valid || x > last_val)
        {
            if (len + 1 > cap)
            {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = x;
            last_val     = x;
            last_valid   = true;
        }

        /* add 2*F(2k-1)-4 when k>1 and it is strictly greater
           than the last stored value (avoids duplicate 0) */
        if (next_k > 1)
        {
            seq_t y = f_curr + f_curr - 4;   /* 2*f_curr - 4 */
            if (y > last_val)
            {
                if (len + 1 > cap)
                {
                    size_t newcap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cap   = newcap;
                }
                cache[len++] = y;
                last_val     = y;
                last_valid   = true;
            }
        }

        next_k++;
    }

    return cache[n - 1];
}
