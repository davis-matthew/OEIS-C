/*
 * A335411  a(n) is the number of vertices formed by n-secting the angles of
 *          an equilateral triangle.
 * https://oeis.org/A335411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-10)-a(n-11)-a(n-12)+a(n-13) for
 *            n>13 initial values: a(1)=3, a(2)=7, a(3)=21, a(4)=25, a(5)=63,
 *            a(6)=67, a(7)=129, a(8)=133, a(9)=219, a(10)=199, a(11)=333,
 *            a(12)=337, a(13)=471
 * Range:     u8 n <= 10, u16 n <= 148, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[14] = {
    0,          /* dummy 0‑th index */
    3, 7, 21, 25, 63, 67, 129, 133, 219, 199, 333, 337, 471
};

static seq_t *memo = NULL;      /* cache for computed values */
static size_t memo_cap = 0;     /* allocated capacity (≥ n+1)   */
static size_t memo_size = 0;    /* maximum index stored + 1     */

/* Ensure memo array can hold values up to index n (1‑based). */
static void _ensure_memo(idx_t n)
{
    if (memo == NULL) {
        /* initial allocation */
        memo_cap = (n + 16) > 32 ? (n + 16) : 32;
        memo = (seq_t *)calloc(memo_cap, sizeof(seq_t));
        /* copy known initial values */
        for (size_t i = 1; i <= 13 && i < memo_cap; ++i)
            memo[i] = _init[i];
        memo_size = 14;                  /* indices 1..13 stored */
    } else if ((size_t)n >= memo_cap) {
        size_t new_cap = memo_cap;
        if (new_cap == 0) new_cap = 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – abort program */
            abort();
        }
        memo = tmp;
        memset(&memo[memo_cap], 0, (new_cap - memo_cap) * sizeof(seq_t));
        memo_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 13) return _init[(size_t)n];

    _ensure_memo(n);

    /* Compute missing terms up to n */
    for (idx_t i = memo_size; i <= n; ++i) {
        seq_t val = memo[i-1] + memo[i-2];
        val -= memo[i-3];
        val += memo[i-10];
        val -= memo[i-11];
        val -= memo[i-12];
        val += memo[i-13];
        memo[i] = val;
    }
    memo_size = (size_t)n + 1;
    return memo[(size_t)n];
}
