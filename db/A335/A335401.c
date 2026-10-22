/*
 * A335401  a(n) is the smallest positive number such that the decimal digits
 *          of n*a(n) are all 0, 1, 2 or 3.
 * https://oeis.org/A335401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A335401: a(n) = smallest k such that n*k has only decimal digits 0,1,2,3.  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    if (n == 0) return 0;   /* safety for callers with 0 indexing */

    /* All data structures depend on n, which is small in the tests
       (≤ 10 000).  We therefore can safely cast n to size_t for
       allocation. */
    size_t sz = (size_t)n;

    /* visited array */
    unsigned char *vis = (unsigned char *)malloc(sz * sizeof(unsigned char));
    /* parent remainder */
    idx_t *par  = (idx_t *)malloc(sz * sizeof(idx_t));
    /* digit that led from parent to this state */
    unsigned char *dig = (unsigned char *)malloc(sz * sizeof(unsigned char));
    /* BFS queue */
    idx_t *q = (idx_t *)malloc(sz * sizeof(idx_t));

    if (!vis || !par || !dig || !q) {
        free(vis); free(par); free(dig); free(q);
        return 0;
    }

    for (size_t i = 0; i < sz; ++i) vis[i] = 0;

    size_t head = 0, tail = 0;

    /* Enqueue the first allowed digits (1,2,3).  If any of them
       already gives a remainder 0 we are done immediately. */
    for (int d = 1; d <= 3; ++d) {
        idx_t r = (idx_t)(d % n);
        if (!vis[r]) {
            vis[r] = 1;
            par[r]  = (idx_t)-1;   /* root */
            dig[r]  = (unsigned char)d;
            q[tail++] = r;

            if (r == 0) {          /* the number is just the single digit d */
                seq_t num = (seq_t)d;
                seq_t k   = num / (seq_t)n;
                free(vis); free(par); free(dig); free(q);
                return k;
            }
        }
    }

    /* BFS over remainders */
    while (head < tail) {
        idx_t r = q[head++];
        for (int d = 0; d <= 3; ++d) {
            idx_t nr = ((r * 10) + d) % n;
            if (!vis[nr]) {
                vis[nr] = 1;
                par[nr] = r;
                dig[nr] = (unsigned char)d;
                q[tail++] = nr;
                if (nr == 0) {
                    /* Found the minimal number. Reconstruct its digits. */
                    /* We'll store them in reverse order in a dynamic array. */
                    size_t cap = 16;
                    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
                    if (!digits) {
                        free(vis); free(par); free(dig); free(q);
                        return 0;
                    }
                    size_t len = 0;
                    idx_t cur = 0;
                    while (1) {
                        if (len == cap) {
                            cap <<= 1;
                            uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
                            if (!tmp) { free(digits); free(vis); free(par); free(dig); free(q); return 0; }
                            digits = tmp;
                        }
                        digits[len++] = dig[cur];
                        idx_t p = par[cur];
                        if (p == (idx_t)-1) break;
                        cur = p;
                    }
                    /* Compute the full number from most significant digit */
                    seq_t num = 0;
                    for (size_t i = 0; i < len; ++i)
                        num = num * 10 + (seq_t)digits[len - 1 - i];
                    seq_t k = num / (seq_t)n;

                    free(digits);
                    free(vis); free(par); free(dig); free(q);
                    return k;
                }
            }
        }
    }

    /* Should never happen for n ≥ 1, but in case no multiple is found */
    free(vis); free(par); free(dig); free(q);
    return 0;
}
