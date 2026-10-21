/*
 * A241183  Start with n; add to it any of its digits; repeat; a(n) = minimal
 *          number of steps needed to reach 2n, or -1 if 2n cannot be reached.
 * https://oeis.org/A241183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = minimal number of digit‑addition steps needed to reach 2n
 *            from n (search via breadth‑first traversal)
 * Range:     i8 n <= 63, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const seq_t UNCOMPUTED = -2;   /* sentinel for memoization */

static seq_t *memo     = NULL;   /* cached results, memo[0] unused */
static idx_t memo_cap   = 0;      /* capacity of memo array */

/* ensure memo has room for at least n+1 entries, initialise new entries to UNCOMPUTED */
static void ensure_memo_cap(idx_t n)
{
    if (n < memo_cap)
        return;
    idx_t new_cap = (memo_cap == 0) ? 64 : memo_cap * 2;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) {
        abort();   /* out of memory */
    }
    /* initialise new slots */
    for (idx_t i = memo_cap; i < new_cap; ++i)
        new_mem[i] = UNCOMPUTED;
    memo      = new_mem;
    memo_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)            /* out of defined range, but undefined behaviour for the task */
        return -1;

    ensure_memo_cap(n);
    if (memo[n] != UNCOMPUTED)
        return memo[n];

    /* BFS from n to 2n */
    seq_t target = (seq_t)n * 2;
    size_t sz = (size_t)target + 1;      /* target is at most 2*n, fits in size_t */

    /* distance array, initialised to -1 (unvisited) */
    int *dist = (int *)malloc(sz * sizeof(int));
    if (!dist) abort();                 /* memory error */
    for (size_t i = 0; i < sz; ++i)
        dist[i] = -1;

    /* simple queue using a fixed array */
    int *queue = (int *)malloc(sz * sizeof(int));
    if (!queue) { free(dist); abort(); }
    size_t q_head = 0, q_tail = 0;

    int start = (int)n;
    dist[start] = 0;
    queue[q_tail++] = start;
    int answer = -1;

    while (q_head < q_tail) {
        int cur = queue[q_head++];
        if (cur == (int)target) {
            answer = dist[cur];
            break;
        }
        /* iterate over digits of cur */
        int tmp = cur;
        while (tmp > 0) {
            int d = tmp % 10;
            tmp /= 10;
            if (d == 0) continue;            /* adding 0 would stay in place */
            int nxt = cur + d;
            if (nxt <= (int)target && dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                queue[q_tail++] = nxt;
            }
        }
    }

    free(dist);
    free(queue);

    seq_t result = (seq_t)answer;          /* answer may be -1 if unreachable */
    memo[n] = result;                     /* cache the computed value */
    return result;
}
