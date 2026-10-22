/*
 * A334963  a(n) is the least positive multiple of n that has at most two
 *          distinct digits.
 * https://oeis.org/A334963
 *
 * Offset:    102    (a(102) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 809, u64 n <= 9759, u128 n <= 9759,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A334963: least multiple of n that has at most two distinct decimal digits */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 20000   /* static array size for small n */

static bool min_with_digits(idx_t n, int d1, int d2, seq_t *res)
{
    /* collect distinct digits, keep at most two */
    int digits[2];
    int nd = 0;
    digits[nd++] = d1;
    if (d2 != d1) digits[nd++] = d2;
    if (digits[0] == 0 && nd == 1) return false;          /* only 0 */

    /* n must fit in an int for array indices */
    if (n == 0) return false;
    const int N = (int)n;
    if (N <= 0) return false;

    /* buffers */
    uint8_t *visited;
    int     *parent;
    uint8_t *dig;
    int     *q;
    bool use_static = (N <= MAXN);

    if (use_static) {
        static uint8_t  visited_static[MAXN + 1];
        static int      parent_static[MAXN + 1];
        static uint8_t  dig_static[MAXN + 1];
        static int      q_static[MAXN + 1];

        visited = visited_static;
        parent  = parent_static;
        dig     = dig_static;
        q       = q_static;
    } else {
        visited = (uint8_t *)calloc((size_t)N, sizeof(uint8_t));
        parent  = (int *)   calloc((size_t)N, sizeof(int));
        dig     = (uint8_t *)calloc((size_t)N, sizeof(uint8_t));
        q       = (int *)   calloc((size_t)N, sizeof(int));
        if (!visited || !parent || !dig || !q) {
            free(visited); free(parent); free(dig); free(q);
            return false;
        }
    }

    memset(visited, 0, (size_t)N * sizeof(uint8_t));
    int head = 0, tail = 0;

    /* enqueue starting digits (non‑zero) */
    for (int i = 0; i < nd; ++i) {
        int d = digits[i];
        if (d == 0) continue;
        int rem = d % N;
        if (!visited[rem]) {
            visited[rem] = 1;
            parent[rem]  = -1;
            dig[rem]     = (uint8_t)d;
            q[tail++]    = rem;
            if (rem == 0) {               /* 1‑digit solution */
                *res = (seq_t)d;
                if (!use_static) {
                    free(visited); free(parent); free(dig); free(q);
                }
                return true;
            }
        }
    }

    while (head < tail) {
        int rem = q[head++];
        for (int i = 0; i < nd; ++i) {
            int d = digits[i];
            seq_t tmp = ((seq_t)rem * 10 + d) % (seq_t)n;
            int nx = (int)tmp;
            if (!visited[nx]) {
                visited[nx] = 1;
                parent[nx]  = rem;
                dig[nx]     = (uint8_t)d;
                q[tail++]   = nx;
                if (nx == 0) {
                    /* reconstruct the number */
                    uint8_t *rev = (uint8_t *)malloc((size_t)tail * sizeof(uint8_t));
                    if (!rev) {
                        if (!use_static) {
                            free(visited); free(parent); free(dig); free(q);
                        }
                        return false;
                    }
                    int pos = 0, cur = 0;
                    while (cur != -1) {
                        rev[pos++] = dig[cur];
                        cur = parent[cur];
                    }
                    seq_t val = 0;
                    for (int j = pos - 1; j >= 0; --j) {
                        val = val * 10 + (seq_t)rev[j];
                    }
                    free(rev);
                    *res = val;
                    if (!use_static) {
                        free(visited); free(parent); free(dig); free(q);
                    }
                    return true;
                }
            }
        }
    }

    if (!use_static) {
        free(visited); free(parent); free(dig); free(q);
    }
    return false;   /* should not happen */
}

seq_t a(idx_t n)
{
    if (n < 102) return 0;   /* offset handling */

    seq_t best = 0;
    bool found = false;

    for (int a = 0; a <= 9; ++a) {
        for (int b = a; b <= 9; ++b) {
            seq_t val;
            if (min_with_digits(n, a, b, &val)) {
                if (!found || val < best) {
                    best = val;
                    found = true;
                }
            }
        }
    }
    return best;
}
