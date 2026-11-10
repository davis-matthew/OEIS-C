/*
 * A257804  Positions of even numbers in A233271, the infinite trunk of
 *          inverted binary beanstalk.
 * https://oeis.org/A257804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   let t(0)=0, t(k+1) = t(k) + 1 + (number of 0's in binary t(k))
 *            (A233271); a(n) = position k of the n-th even term t(k) (n >=
 *            0), found by scanning
 * Range:     u8 n <= 140, u16 n <= 8433, u32 n <= 8433, u64 n <= 8433,
 *            u128 n <= 8433, big any n
 * Validated: standard, pass, 94 terms through n = 7298 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A257804_cache = NULL;
static idx_t A257804_cap = 0;
static idx_t A257804_cnt = 0;
static uint64_t A257804_cur = 0;
static idx_t A257804_pos = 0;

seq_t a(idx_t n)
{
    while (A257804_cnt <= n)
    {
        if ((A257804_cur & 1) == 0)
        {
            if (A257804_cnt + 1 > A257804_cap)
            {
                idx_t nc = A257804_cap ? A257804_cap * 2 : 64;
                while (nc < A257804_cnt + 1)
                    nc *= 2;
                A257804_cache = mem_resize(seq_t, A257804_cache, (size_t)nc);
                A257804_cap = nc;
            }
            A257804_cache[A257804_cnt++] = (seq_t)A257804_pos;
        }
        uint64_t t = A257804_cur, zeros = 0;
        while (t)
        {
            if ((t & 1) == 0)
                zeros++;
            t >>= 1;
        }
        A257804_cur = A257804_cur + 1 + zeros;
        A257804_pos++;
    }
    return A257804_cache[n];
}
