/*
 * A257803  Positions of odd numbers in A233271, the infinite trunk of
 *          inverted binary beanstalk.
 * https://oeis.org/A257803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k of the n-th odd term in A233271, where A233271 is
 *            b(0)=0, b(k+1) = b(k) + 1 + (number of 0's in binary expansion
 *            of b(k)); computed by direct iteration
 * Range:     u8 n <= 30, u16 n <= 4764, u32 n <= 7973, u64 n <= 7973,
 *            u128 n <= 7973, big any n
 * Validated: standard, pass, 90 terms through n = 7892 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static idx_t *A257803_pos = NULL;
static idx_t A257803_cap = 0;
static idx_t A257803_cnt = 0;
static seq_t A257803_cur = 0;
static idx_t A257803_idx = 0;

static void A257803_push(idx_t v)
{
    if (A257803_cnt >= A257803_cap)
    {
        idx_t nc = A257803_cap ? A257803_cap * 2 : 1024;
        A257803_pos = mem_resize(idx_t, A257803_pos, nc);
        A257803_cap = nc;
    }
    A257803_pos[A257803_cnt++] = v;
}

seq_t a(idx_t n)
{
    while (A257803_cnt < n)
    {
        seq_t t = A257803_cur;
        seq_t zeros = 0;
        while (t)
        {
            if ((t & 1) == 0)
                zeros++;
            t >>= 1;
        }
        A257803_cur = A257803_cur + 1 + zeros;
        A257803_idx++;
        if ((A257803_cur & 1) == 1)
            A257803_push(A257803_idx);
    }
    return (seq_t)A257803_pos[n - 1];
}
