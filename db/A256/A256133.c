/*
 * A256133  Numbers that have unique expansion with minimal digit sum in terms
 *          of Fibonacci numbers F_k (k > 1).
 * https://oeis.org/A256133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer whose Zeckendorf expansion (using Fibonacci
 *            numbers F_k, k > 1) contains no substring 1001 and does not end
 *            in 101. The function generates the numbers in order and memoises
 *            them so that subsequent calls return the value instantly
 * Range:     u8 n <= 2, u16 n <= 141, u32 n <= 141, u64 n <= 141,
 *            u128 n <= 141, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static Fibonacci table (k > 1 : 1, 2, 3, 5, 8, ...) */
#define MAX_FIB 200
static seq_t fib[MAX_FIB];
static size_t fib_cnt = 0;

/* initialise fib[] once */
static void init_fib(void)
{
    if (fib_cnt != 0) return;          /* already initialised */

    fib[0] = 1;                        /* F_2 */
    fib[1] = 2;                        /* F_3 */
    fib_cnt = 2;

    /* precompute a generous number of Fibonacci numbers */
    while (fib_cnt < MAX_FIB) {
        seq_t next = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        fib[fib_cnt++] = next;
    }
}

/* test whether m has a Zeckendorf expansion without “1001” and without trailing “101” */
static bool valid(seq_t m)
{
    bool bits[MAX_FIB] = { false };
    seq_t rem = m;
    size_t idx = fib_cnt - 1;

    /* greedy Zeckendorf construction */
    while (rem > (seq_t)0) {
        while (fib[idx] > rem) idx--;
        bits[idx] = true;
        rem -= fib[idx];
        if (idx == 0) break;
        idx--;
    }

    /* avoid “…1001…” */
    for (size_t i = 3; i < fib_cnt; ++i) {
        if (bits[i] && !bits[i - 1] && !bits[i - 2] && bits[i - 3])
            return false;
    }

    /* avoid “…101” at the end */
    if (fib_cnt > 2 && bits[2] && !bits[1] && bits[0])
        return false;

    return true;
}

/* memoised sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for 0‑index */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_val = 1;          /* first integer to test */

    init_fib();                         /* lazily initialise Fibonacci table */

    /* grow cache if required */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    /* generate terms until we have the n-th one */
    while (cache_len < (size_t)n) {
        while (!valid(next_val)) next_val++;   /* skip invalid numbers */

        /* store valid number */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = next_val;
        next_val++;                           /* advance to next candidate */
    }

    return cache[n - 1];
}
