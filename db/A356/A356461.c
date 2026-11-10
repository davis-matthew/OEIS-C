/*
 * A356461  Expansion of e.g.f. ( Product_{k>0} B(x^k)^k )^(1/(1-x)) where
 *          B(x) = exp(exp(x)-1) = e.g.f. of Bell numbers.
 * https://oeis.org/A356461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} A356459(k) * binomial(n-1,k-1) *
 *            a(n-k), where A356459(k) = k! * Sum_{j=1..k} Sum_{d|j} d/(j/d)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A356459_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t ratio = (seq_t)1;
    seq_t sum   = (seq_t)0;

    for (idx_t m = n; m > 0; --m)
    {
        idx_t t = n / m;
        seq_t tt = ((seq_t)t * (seq_t)(t + 1)) / (seq_t)2;
        sum += ratio * tt;
        if (m > 1)
            ratio *= (seq_t)m;
    }
    return sum;
}

static seq_t *A356461_memo = 0;
static seq_t *A356461_av = 0;
static seq_t *A356461_row = 0;
static size_t A356461_cnt = 0;
static size_t A356461_cap = 0;

static void A356461_grow(size_t need)
{
    if (need <= A356461_cap)
        return;
    size_t ncap = A356461_cap ? A356461_cap : 32;
    while (ncap < need)
        ncap *= 2;
    seq_t *m2 = mem_new(seq_t, ncap);
    seq_t *a2 = mem_new(seq_t, ncap);
    seq_t *r2 = mem_new(seq_t, ncap);
    for (size_t i = 0; i < A356461_cnt; i++)
    {
        m2[i] = A356461_memo[i];
        a2[i] = A356461_av[i];
        r2[i] = A356461_row[i];
    }
    if (A356461_memo)
    {
        mem_free(A356461_memo);
        mem_free(A356461_av);
        mem_free(A356461_row);
    }
    A356461_memo = m2;
    A356461_av = a2;
    A356461_row = r2;
    A356461_cap = ncap;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    A356461_grow(N + 2);
    while (A356461_cnt <= N)
    {
        size_t m = A356461_cnt;
        if (m == 0)
        {
            A356461_memo[0] = (seq_t)1;
            A356461_av[0] = (seq_t)0;
            A356461_row[0] = (seq_t)1;   /* C(0,0) */
        }
        else
        {
            A356461_av[m] = A356459_a((idx_t)m);
            seq_t s = (seq_t)0;
            for (size_t k = 1; k <= m; k++)
                s += A356461_av[k] * A356461_row[k - 1] * A356461_memo[m - k];
            A356461_memo[m] = s;
            /* update row to C(m, 0..m) */
            A356461_row[m] = (seq_t)1;
            for (size_t j = m - 1; j >= 1; j--)
                A356461_row[j] += A356461_row[j - 1];
        }
        A356461_cnt++;
    }
    return A356461_memo[N];
}
