/*
 * A331164  Number of function evaluations when recursively calculating
 *          Fibonacci(n) with caching.
 * https://oeis.org/A331164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of function evaluations when recursively calculating Fibonacci(n) with caching. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *vis = NULL;   /* visited flags (0 = not yet computed) */
static size_t vis_cap = 0;          /* capacity of vis array */
static idx_t *stk = NULL;           /* stack for iterative traversal */
static size_t stk_cap = 0;          /* capacity of stack */

/* Ensure storage is large enough for indices up to n. The stack is
 * allocated 4× the needed space so a node may push up to three children. */
static void ensure_cap(idx_t n)
{
    size_t need_vis = (size_t)(n) + 1;
    if (vis_cap < need_vis) {
        unsigned char *tmp = (unsigned char *)realloc(vis, need_vis);
        if (!tmp) abort();               /* allocation failed */
        vis = tmp;
        memset(vis + vis_cap, 0, need_vis - vis_cap);
        vis_cap = need_vis;
    }
    size_t need_stk = (size_t)(n + 1) * 4;
    if (stk_cap < need_stk) {
        idx_t *tmp = (idx_t *)realloc(stk, need_stk * sizeof(idx_t));
        if (!tmp) abort();
        stk = tmp;
        stk_cap = need_stk;
    }
}

seq_t a(idx_t n)
{
    ensure_cap(n);

    size_t top = 0;          /* stack top index */
    stk[top++] = n;          /* start with the requested n */
    seq_t cnt = 0;           /* number of function evaluations */

    while (top) {
        idx_t cur = stk[--top];
        cnt++;  /* count this function call */

        if (vis[cur])          /* already computed, no further recursion */
            continue;

        vis[cur] = 1;          /* mark as computed */

        if (cur <= 2)          /* base cases: no further recursion */
            continue;

        if ((cur & 1) == 1) {  /* n is odd */
            idx_t a_val = (cur + 1) >> 1;
            idx_t b_val = (cur - 1) >> 1;
            stk[top++] = a_val;
            stk[top++] = b_val;
        } else {               /* n is even */
            idx_t half = cur >> 1;
            stk[top++] = half;
            if (half > 0) stk[top++] = half - 1;
            stk[top++] = half + 1;
        }
    }

    /* Clear visited flags for the next call. */
    for (idx_t i = 0; i <= n; ++i)
        vis[i] = 0;

    return cnt;
}
