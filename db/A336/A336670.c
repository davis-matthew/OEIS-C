/*
 * A336670  Numbers that have decimal expansion c(1)c(2)...c(n) with distinct
 *          digits that satisfy c(1) <> 0, c(1) is the largest digit, and for
 *          each i in 1..n there is j in {0, 1} such that c(i) == 2*c(i-1) + j
 *          (mod 10) (with c(0): = c(n)).
 * https://oeis.org/A336670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   numbers that form a directed simple cycle in digits 0..9 with
 *            the property that each successive digit is (2*previous + j) mod
 *            10, j∈{0,1}, all digits distinct, first digit is the largest,
 *            leading digit not 0. The function enumerates all such cycles,
 *            converts each to a decimal integer (starting at...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"

static seq_t *seqs   = NULL;   /* array of terms, sorted ascending */
static idx_t seq_cap = 0;
static idx_t seq_len = 0;

static int path[10];  /* current path during DFS */

/* append a term to the dynamic array */
static void push_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 4;
        seqs = (seq_t *)realloc(seqs, seq_cap * sizeof(seq_t));
    }
    seqs[seq_len++] = val;
}

/* record the cycle described by path[0..depth-1] */
static void record_cycle(int depth)
{
    int max_digit = -1, max_index = 0;
    for (int i = 0; i < depth; ++i) {
        if (path[i] > max_digit) {
            max_digit   = path[i];
            max_index   = i;
        }
    }
    seq_t val = 0;
    for (int i = 0; i < depth; ++i) {
        int d = path[(max_index + i) % depth];
        val = val * 10 + (seq_t)d;
    }
    push_seq(val);
}

/* depth‑first search for directed simple cycles starting at `start` */
static void dfs(int start, int curr, int depth, int visited)
{
    for (int offset = 0; offset <= 1; ++offset) {
        int next = (2 * curr + offset) % 10;
        if (next == start) {
            record_cycle(depth);
        } else if (!(visited & (1 << next)) && depth < 10) {
            path[depth] = next;
            dfs(start, next, depth + 1, visited | (1 << next));
        }
    }
}

/* compute all terms of the sequence */
static void compute_sequence(void)
{
    /* enumerate cycles for every possible start digit */
    for (int start = 0; start < 10; ++start) {
        path[0] = start;
        dfs(start, start, 1, 1 << start);
    }

    /* sort terms and remove duplicates */
    qsort(seqs, seq_len, sizeof(seq_t), cmp_seq_t);
    idx_t uniq = 0;
    for (idx_t i = 0; i < seq_len; ++i) {
        if (uniq == 0 || seqs[i] != seqs[uniq - 1]) {
            seqs[uniq++] = seqs[i];
        }
    }
    seq_len = uniq;
}

seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready) {
        compute_sequence();
        ready = true;
    }
    if (n == 0 || n > seq_len)  /* outside known range */
        return 0;
    return seqs[n - 1];
}
