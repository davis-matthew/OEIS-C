/*
 * A244957  Smallest positive multiple of n whose base-6 representation
 *          contains only 0's and 1's.
 * https://oeis.org/A244957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 214, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): smallest multiple of n whose base‑6 representation
   consists only of 0's and 1's.  The algorithm performs a breadth‑first
   search over the residues modulo n. */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        seq_t v = 1;
        return v;
    }

    size_t sz = (size_t)n;
    bool *vis = (bool*)calloc(sz, sizeof(bool));
    int  *par = (int*)malloc(sz * sizeof(int));
    char *dig = (char*)malloc(sz * sizeof(char));
    size_t *q   = (size_t*)malloc(sz * sizeof(size_t));

    for (size_t i = 0; i < sz; ++i) par[i] = -1;

    size_t start = (size_t)(1 % n);          /* leading digit is 1 */
    vis[start] = true;
    dig[start] = 1;
    size_t head = 0, tail = 0;
    q[tail++] = start;

    /* Breadth‑first search on residues */
    while (head < tail) {
        size_t r = q[head++];
        unsigned long long rr = r;
        for (int d = 0; d <= 1; ++d) {
            size_t r2 = (rr * 6 + d) % n;
            if (!vis[r2]) {
                vis[r2] = true;
                par[r2] = (int)r;
                dig[r2] = (char)d;
                q[tail++] = r2;
                if (r2 == 0) goto found;   /* finished */
            }
        }
    }

found:
    /* Reconstruct the digit sequence for remainder 0 */
    size_t *rev = (size_t*)malloc(sz * sizeof(size_t));
    size_t revlen = 0;
    size_t cur = 0;
    while ((size_t)par[cur] != (size_t)-1) {   /* go until the start residue */
        rev[revlen++] = dig[cur];
        cur = (size_t)par[cur];
    }
    rev[revlen++] = dig[cur];                  /* leading digit */

    /* Build the final value from digits (most significant first) */
    seq_t val = 0;
    for (size_t i = revlen; i > 0; --i) {
        int d = (int)rev[i - 1];
        val = val * 6 + d;
    }

    /* Clean up */
    free(vis);
    free(par);
    free(dig);
    free(q);
    free(rev);

    return val;
}
