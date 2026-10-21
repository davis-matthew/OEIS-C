/*
 * A261769  Number of distinct representations of n as a product of nonunit
 *          Fibonacci numbers (order doesn't matter).
 * https://oeis.org/A261769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct unordered factorizations of n into
 *            Fibonacci numbers > 1
 * Range:     u8 n <= 9975, u16 n <= 9975, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *fib_vals = NULL;
static size_t fib_count = 0;
static seq_t *memo = NULL;
static bool *vis = NULL;
static idx_t current_limit = 0;
static size_t current_fib_cnt = 0;
static size_t current_rows = 0;

static void rebuild_fibs(idx_t limit) {
    free(fib_vals);
    fib_vals = NULL;
    fib_count = 0;
    uint64_t a = 1, b = 1;
    while (true) {
        uint64_t c = a + b;
        a = b; b = c;
        if (c > limit) break;
        if (c < 2) continue;
        fib_vals = (uint64_t*)realloc(fib_vals, (fib_count + 1) * sizeof(uint64_t));
        fib_vals[fib_count++] = c;
    }
}

static void ensure_prepared(idx_t n) {
    if (n <= current_limit) return;                 /* already prepared */
    rebuild_fibs(n);
    size_t new_fib_cnt = fib_count;
    size_t new_rows    = (size_t)n + 1;
    size_t new_total   = new_rows * new_fib_cnt;

    seq_t *new_memo = (seq_t*)calloc(new_total, sizeof(seq_t));
    bool  *new_vis  = (bool*)calloc(new_total, sizeof(bool));

    if (memo) {
        size_t rows_to_copy = current_rows;
        if (rows_to_copy > new_rows) rows_to_copy = new_rows;
        for (size_t r = 0; r < rows_to_copy; ++r) {
            size_t old_offset = r * current_fib_cnt;
            size_t new_offset = r * new_fib_cnt;
            memcpy(new_memo + new_offset, memo + old_offset,
                   current_fib_cnt * sizeof(seq_t));
            memcpy(new_vis  + new_offset, vis  + old_offset,
                   current_fib_cnt * sizeof(bool));
        }
        free(memo);
        free(vis);
    }

    memo = new_memo;
    vis  = new_vis;
    current_limit   = n;
    current_rows    = new_rows;
    current_fib_cnt = new_fib_cnt;
}

static seq_t ways(idx_t n, size_t min_index) {
    if (n == 1) return 1;
    if (min_index >= fib_count) return 0;
    size_t idx = ((size_t)n) * fib_count + min_index;
    if (vis[idx]) return memo[idx];
    seq_t total = 0;
    for (size_t i = min_index; i < fib_count; ++i) {
        uint64_t f = fib_vals[i];
        if (f > n) break;
        if (n % f == 0) {
            total += ways(n / f, i);
        }
    }
    memo[idx] = total;
    vis[idx]  = true;
    return total;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_prepared(n);
    return ways(n, 0);
}
