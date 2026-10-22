/*
 * A315271  Coordination sequence Gal.6.345.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) a(0)=1, a(1)=6, a(2)=10, a(3)=15, a(4)=21, a(5)=25,
 *            a(6)=30, a(7)=35, a(8)=39, a(9)=45, a(10)=50
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *memo = NULL;          /* storage for computed values      */
static size_t memo_cap = 0;         /* current capacity of memo array   */
static size_t memo_len = 0;         /* number of initialized entries    */

static const seq_t init_vals[11] = {1, 6, 10, 15, 21, 25, 30, 35, 39, 45, 50};

/* Ensure memo array can hold at least 'needed' entries */
static void ensure_capacity(size_t needed)
{
    if (memo_cap >= needed)
        return;

    size_t new_cap = memo_cap ? memo_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;                /* grow until large enough */

    seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_mem)                      /* memory allocation failed */
        exit(EXIT_FAILURE);

    if (memo) {
        for (size_t i = 0; i < memo_cap; ++i)
            new_mem[i] = memo[i];
        free(memo);
    }
    memo = new_mem;
    memo_cap = new_cap;
}

/* Initialise the first 11 values in the memo array */
static void init_memo_prefix(void)
{
    for (size_t i = 0; i < 11; ++i)
        memo[i] = init_vals[i];
    memo_len = 11;
}

/* Compute a(n) with memoisation; accepts any order of calls */
seq_t a(idx_t n)
{
    /* Return the trivial values directly */
    if ((size_t)n < 11)
        return init_vals[(size_t)n];

    /* Preparations */
    size_t idx = (size_t)n;
    ensure_capacity(idx + 1);
    if (memo_len == 0)
        init_memo_prefix();

    /* If the value is already computed, return it immediately */
    if (idx < memo_len)
        return memo[idx];

    /* Compute new values sequentially up to idx */
    for (size_t i = memo_len; i <= idx; ++i) {
        seq_t val = memo[i - 1]
                  + memo[i - 3]
                  - memo[i - 4]
                  - memo[i - 6]
                  + memo[i - 7]
                  + memo[i - 9]
                  - memo[i - 10];
        memo[i] = val;
    }
    memo_len = idx + 1;
    return memo[idx];
}
