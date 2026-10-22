/*
 * A316147  Numbers, with digits in nondecreasing order, whose product of
 *          digits equals the number of digits times the sum of digits.
 * https://oeis.org/A316147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth number whose digits are nondecreasing, and for
 *            which (product of digits) == (#digits) * (sum of digits). The
 *            sequence starts: 0,1,2,3,4,5,6,7,8,9,36,44,159,167,... This
 *            implementation generates the numbers by a depth‑first search
 *            over digit combinations, storing all valid results...
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* array of computed terms */
static size_t seq_len = 0;          /* number of terms stored */
static size_t seq_cap = 0;          /* allocated capacity of seq[] */
static idx_t   max_len = 0;         /* maximum digit length generated */

/* Ensure the array can hold at least 'min_cap' elements. */
static void seq_grow(size_t min_cap)
{
    if (min_cap <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Depth‑first search that builds all nondecreasing‑digit numbers of
   length 'target_len'.  'depth' is the current position (0‑based).
   'val'   – current numeric value,
   'prod'  – product of digits seen so far,
   'sum'   – sum of digits seen so far,
   'min_d' – smallest digit that may be placed next (to enforce nondecreasing). */
static void dfs(idx_t depth, idx_t target_len,
                seq_t val, seq_t prod, seq_t sum,
                uint8_t min_d)
{
    if (depth == target_len) {
        if (prod == (seq_t)target_len * sum) {
            seq_grow(seq_len + 1);
            seq[seq_len++] = val;
        }
        return;
    }

    for (uint8_t d = min_d; d <= 9; ++d) {
        seq_t new_val  = val * 10 + d;
        seq_t new_prod = prod * d;
        seq_t new_sum  = sum + d;
        dfs(depth + 1, target_len, new_val, new_prod, new_sum, d);
    }
}

/* Main sequence function.  Generates terms as needed and returns the nth term. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but guard against misuse */

    /* Initialize the static array with the first term 0 (a(1)). */
    if (seq_len == 0) {
        seq_grow(1);
        seq[0] = 0;
        seq_len = 1;
        max_len = 0;
    }

    /* Expand the precomputed list until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        for (idx_t L = max_len + 1; ; ++L) {
            dfs(0, L, 0, 1, 0, 1);   /* generate all length-L numbers */
            if (seq_len >= (size_t)n) {
                max_len = L;
                break;
            }
        }
    }

    return seq[n - 1];   /* offset 1 → index n-1 */
}
