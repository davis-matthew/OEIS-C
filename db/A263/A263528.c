/*
 * A263528  Expansion of (psi(x) * psi(x^3) / f(-x^3)^2)^2 in powers of x
 *          where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A263528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Euler transform of period‑6 sequence [ 2, -2, 8, -2, 2,
 *            0, … ]
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 88, u64 n <= 297,
 *            u128 n <= 1057, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *seq = NULL;
    static size_t cap = 0;          /* current allocated length of seq   */

    size_t N = (size_t)n;           /* index we need to compute up to   */
    if ( (idx_t)cap <= n )
    {
        /* resize cache */
        size_t new_cap = N + 1;
        seq_t *new_seq = seq_alloc_new_array(new_cap);
        if (seq != NULL)
        {
            for (size_t i = 0; i < cap; ++i)
                new_seq[i] = seq[i];
            seq_free_array(seq);
        }
        seq = new_seq;
        cap = new_cap;

        /* t[k]  =  Σ_{d|k} d * b(d)   with b period 6            */
        int *t = (int*)malloc((N + 1) * sizeof(int));
        if (t == NULL) return 0;
        for (size_t i = 0; i <= N; ++i) t[i] = 0;

        static const int pat[6] = { 2, -2, 8, -2, 2, 0 };

        for (size_t d = 1; d <= N; ++d)
        {
            int bval = pat[(d - 1) % 6];
            if (bval == 0) continue;
            int contrib = bval * (int)d;
            for (size_t m = d; m <= N; m += d)
                t[m] += contrib;
        }

        /* Fill first term */
        seq[0] = (seq_t)1;

        /* compute remaining terms using the Euler recursion */
        for (size_t nn = 1; nn <= N; ++nn)
        {
            if (sizeof(seq_t) == 8)  /* 64‑bit build – use 128‑bit intermediates */
            {
                __int128 acc = 0;
                for (size_t k = 1; k <= nn; ++k)
                {
                    int val = t[k];
                    seq_t term = seq[nn - k];
                    unsigned long long abs_val = (val >= 0) ? (unsigned long long)val
                                                             : (unsigned long long)(-val);
                    __uint128_t prod = ( __uint128_t )term * abs_val;
                    if (val >= 0) acc += prod;  else acc -= prod;
                }
                seq[nn] = (seq_t)(acc / ( __int128 )nn);
            }
            else                     /* arbitrary‑precision build – use built‑in ops */
            {
                seq_t acc = 0;
                for (size_t k = 1; k <= nn; ++k)
                {
                    int val = t[k];
                    seq_t term = seq[nn - k];
                    if (val >= 0)
                        acc += term * (seq_t)val;
                    else
                        acc -= term * (seq_t)(-val);
                }
                seq[nn] = acc / (seq_t)nn;
            }
        }
        free(t);
    }

    return seq[n];
}
