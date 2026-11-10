/*
 * A269391  Permutation of natural numbers: a(1) = 1, a(A233271(1+n)) =
 *          2*a(n), a(A269390(n)) = 1 + 2*a(n), where A233271 is the infinite
 *          trunk of inverted binary beanstalk and A269390 is its complement.
 * https://oeis.org/A269391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(T(j+1)) = 2*a(j), a(c(k)) = 1 + 2*a(k), where T =
 *            A233271 (trunk, T(0)=0, T(n+1)=T(n)+1+zeros(T(n))) and c = its
 *            complement, computed by sieving m = 2..n in order
 * Range:     u8 n <= 77, u16 n <= 789, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9957 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A269391_arr = NULL;
static idx_t A269391_cap = 0;
static idx_t A269391_done = 0;
static idx_t A269391_tnext = 2;
static idx_t A269391_tj = 2;
static idx_t A269391_kc = 0;

static idx_t A269391_zeros(idx_t x)
{
    idx_t z = 0;
    while (x)
    {
        if ((x & 1) == 0)
            z++;
        x >>= 1;
    }
    return z;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    if (A269391_arr == NULL)
    {
        A269391_cap = 16;
        A269391_arr = mem_new(seq_t, (size_t)A269391_cap);
        A269391_arr[1] = 1;
        A269391_done = 1;
    }
    if (n > A269391_done)
    {
        if (n + 1 > A269391_cap)
        {
            idx_t nc = A269391_cap * 2;
            if (nc < n + 1)
                nc = n + 1;
            A269391_arr = mem_resize(seq_t, A269391_arr, (size_t)nc);
            A269391_cap = nc;
        }
        for (idx_t m = A269391_done + 1; m <= n; ++m)
        {
            if (m == A269391_tnext)
            {
                A269391_arr[m] = (seq_t)2 * A269391_arr[A269391_tj - 1];
                A269391_tnext = A269391_tnext + 1 + A269391_zeros(A269391_tnext);
                A269391_tj++;
            }
            else
            {
                A269391_kc++;
                A269391_arr[m] = (seq_t)1 + (seq_t)2 * A269391_arr[A269391_kc];
            }
        }
        A269391_done = n;
    }
    return A269391_arr[n];
}
