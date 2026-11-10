/*
 * A348458  Partial sums of A307720.
 * https://oeis.org/A348458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A307720(k); A307720 built greedily (search):
 *            smallest x so that product with previous term k has fewer than k
 *            occurrences so far
 * Range:     u8 n <= 78, u16 n <= 6375, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9706 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A348458_ps = NULL;   /* prefix sums */
static size_t A348458_len = 0, A348458_cap = 0;
static seq_t *A348458_cnt = NULL;
static size_t A348458_ccap = 0;
static seq_t A348458_prev = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A348458_len == 0) {
        A348458_cap = 64;
        A348458_ps = mem_new(seq_t, A348458_cap);
        A348458_ccap = 64;
        A348458_cnt = mem_new(seq_t, A348458_ccap);
        A348458_ps[0] = (seq_t)1;
        A348458_prev = (seq_t)1;
        A348458_len = 1;
    }
    while ((idx_t)A348458_len < n) {
        seq_t x = (seq_t)1;
        for (;;) {
            seq_t prod = A348458_prev * x;
            size_t pi = (size_t)(long long)prod;
            if (pi >= A348458_ccap) {
                size_t nc = A348458_ccap;
                while (nc <= pi) nc <<= 1;
                A348458_cnt = mem_resize(seq_t, A348458_cnt, nc);
                for (size_t i = A348458_ccap; i < nc; ++i) A348458_cnt[i] = (seq_t)0;
                A348458_ccap = nc;
            }
            if (A348458_cnt[pi] < prod) {
                A348458_cnt[pi]++;
                if (A348458_len >= A348458_cap) {
                    A348458_cap <<= 1;
                    A348458_ps = mem_resize(seq_t, A348458_ps, A348458_cap);
                }
                A348458_ps[A348458_len] = A348458_ps[A348458_len - 1] + x;
                A348458_len++;
                A348458_prev = x;
                break;
            }
            x++;
        }
    }
    return A348458_ps[(size_t)(n - 1)];
}
