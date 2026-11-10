/*
 * A240912  Numbers not in A240911.
 * https://oeis.org/A240912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A240911; computed by merging
 *            the increasing sequence A240911 (a(1)=1, a(k)=smallest multiple
 *            of least non-divisor of a(k-1) exceeding a(k-1)) against the
 *            integers
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9355 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A240912_vals = NULL;
static size_t A240912_cnt = 0;
static size_t A240912_cap = 0;
static seq_t A240912_cur = 1;   /* current A240911 term */
static seq_t A240912_v = 1;     /* next candidate integer */

static void A240912_push(seq_t x)
{
    if (A240912_cnt == A240912_cap) {
        size_t nc = A240912_cap ? A240912_cap * 2 : 64;
        seq_t *p = mem_resize(seq_t, A240912_vals, nc);
        if (!p) exit(1);
        A240912_vals = p;
        A240912_cap = nc;
    }
    A240912_vals[A240912_cnt++] = x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A240912_cnt < (size_t)n) {
        if (A240912_v == A240912_cur) {
            seq_t k = 2;
            while ((A240912_cur % k) == 0) ++k;
            A240912_cur = (A240912_cur / k + 1) * k;
        } else {
            A240912_push(A240912_v);
        }
        ++A240912_v;
    }
    return A240912_vals[(size_t)n - 1];
}
