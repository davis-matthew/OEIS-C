/*
 * A377472  First differences of Colombian or self numbers A003052.
 * https://oeis.org/A377472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003052(n+1) – A003052(n) (= first differences of
 *            Colombian numbers)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Sum of decimal digits of a 64‑bit unsigned integer. */
static uint64_t digit_sum_u64(uint64_t x)
{
    uint64_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Return the n‑th term of OEIS A377472 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;          /* undefined for n = 0; return 0 as a safe fallback */

    /* Persistent state for the least‑cost incremental generation. */
    static seq_t  *col_vals = NULL;   /* array of Colombian numbers */
    static size_t  col_cnt  = 0;      /* how many have been stored */
    static size_t  col_cap  = 0;      /* current capacity of col_vals */

    static unsigned char *mark   = NULL;   /* 0 = not generated, 1 = generated */
    static size_t         mark_cap = 0;     /* capacity of mark array */

    static uint64_t cur_k    = 1;   /* next k to process for generating sums */
    static uint64_t cur_val  = 1;   /* candidate for next Colombian number */

    /* How many Colombian numbers are needed (index n uses element n+1). */
    const size_t target = (size_t)n + 1U;

    /* Generate until we have at least 'target' Colombian numbers. */
    while (col_cnt < target) {
        /* Ensure that all k ≤ cur_val have been processed and their sums marked. */
        while (cur_k <= cur_val) {
            uint64_t g = cur_k + digit_sum_u64(cur_k);
            if (g >= mark_cap) {
                size_t new_cap = mark_cap ? mark_cap * 2U : 1024U;
                while (new_cap <= g)
                    new_cap *= 2U;
                unsigned char *tmp = (unsigned char *)realloc(mark, new_cap);
                if (!tmp) abort();
                memset(tmp + mark_cap, 0, new_cap - mark_cap);
                mark     = tmp;
                mark_cap = new_cap;
            }
            mark[(size_t)g] = 1U;
            ++cur_k;
        }

        /* cur_val is a candidate Colombian number if it is not marked. */
        if (cur_val >= mark_cap || mark[(size_t)cur_val] == 0U) {
            /* Store it, expanding the array if necessary. */
            if (col_cnt >= col_cap) {
                size_t new_cap = col_cap ? col_cap * 2U : 16U;
                seq_t *tmp = (seq_t *)realloc(col_vals, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                col_vals = tmp;
                col_cap  = new_cap;
            }
            col_vals[col_cnt++] = (seq_t)cur_val;
        }
        ++cur_val;
    }

    /* Compute a(n) = A003052(n+1) – A003052(n). */
    return col_vals[n] - col_vals[n - 1U];
}
