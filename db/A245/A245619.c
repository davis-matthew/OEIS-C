/*
 * A245619  Row sums of triangle A245618.
 * https://oeis.org/A245619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} H(n,k), with H(n,0)=H(n,n)=1 and, for 0<k<n,
 *            H(n,k) = |H(n-1,k) + (-1)^(H(n-1,k)+H(n-1,k-1)) * H(n-1,k-1)|
 *            (A245618)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 39, u64 n <= 74, u128 n <= 141,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 451 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *a245619_row = NULL;
static seq_t *a245619_sums = NULL;
static size_t a245619_cap = 0;
static size_t a245619_done = 0; /* number of rows computed */

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N + 1 > a245619_cap) {
        size_t nc = a245619_cap ? a245619_cap : 16;
        while (nc < N + 1) nc *= 2;
        seq_t *nr = mem_new(seq_t, nc);
        seq_t *ns = mem_new(seq_t, nc);
        for (size_t i = 0; i < a245619_done; ++i) {
            nr[i] = a245619_row[i];
            ns[i] = a245619_sums[i];
        }
        if (a245619_row) mem_free(a245619_row);
        if (a245619_sums) mem_free(a245619_sums);
        a245619_row = nr;
        a245619_sums = ns;
        a245619_cap = nc;
    }
    while (a245619_done <= N) {
        size_t r = a245619_done;
        if (r == 0) {
            a245619_row[0] = 1;
        } else {
            a245619_row[r] = 1;
            for (size_t k = r - 1; k >= 1; --k) {
                seq_t x = a245619_row[k];
                seq_t y = a245619_row[k - 1];
                seq_t m = x + y;
                if ((m & 1) == 0)
                    a245619_row[k] = x + y;
                else
                    a245619_row[k] = x > y ? x - y : y - x;
            }
        }
        seq_t s = 0;
        for (size_t k = 0; k <= r; ++k) s += a245619_row[k];
        a245619_sums[r] = s;
        ++a245619_done;
    }
    return a245619_sums[N];
}
