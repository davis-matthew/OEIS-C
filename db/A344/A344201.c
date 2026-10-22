/*
 * A344201  Number of cyclic subgroups of the group (C_n)^n, where C_n is the
 *          cyclic group of order n.
 * https://oeis.org/A344201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e‖n} [ 1 + (p^n-1)/(p-1) *
 *            (p^{(n-1)e}-1)/(p^{n-1}-1) ] (n ≥ 2), a(1) = 1 Derived from
 *            b(d,n) = (∑_{t|d} μ(d/t) t^n)/φ(d) with b multiplicativity
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_spf.h"

static idx_t *g_spf = NULL;       /* smallest prime factor array */
static idx_t  g_spf_size = 0;     /* current size of spf array */

/* Ensure that spf array covers all numbers up to n. */
static void ensure_spf(idx_t n)
{
    if (g_spf == NULL || g_spf_size <= n) {
        if (g_spf != NULL)
            free(g_spf);
        /* build_spf will allocate a new array of size limit+1 */
        build_spf(n, &g_spf);
        g_spf_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* factor n using smallest prime factor table */
    ensure_spf(n);
    idx_t m = n;
    /* at most log2(n) distinct primes; 64 is ample */
    idx_t primes[64];
    idx_t exps[64];
    size_t pc = 0;
    while (m > 1) {
        idx_t p = g_spf[m];
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            e++;
        }
        primes[pc] = p;
        exps[pc] = e;
        pc++;
    }

    /* compute the product of S_p contributions */
    seq_t result = 1;
    for (size_t i = 0; i < pc; ++i) {
        idx_t p = primes[i];
        idx_t e = exps[i];

        /* (p^n - 1) / (p - 1) */
        seq_t p_pow_n = big_pow((seq_t)p, (seq_t)n);
        seq_t num1 = p_pow_n - 1;
        seq_t denom1 = (seq_t)(p - 1);
        seq_t factor1 = num1 / denom1;      /* integer division */

        /* (p^{(n-1)e} - 1) / (p^{n-1} - 1) */
        idx_t exp_mul = (n - 1) * e;        /* <= 140k for n ≤ 10000 */
        seq_t p_pow_r = big_pow((seq_t)p, (seq_t)exp_mul);
        seq_t num2 = p_pow_r - 1;
        seq_t r_minus1 = big_pow((seq_t)p, (seq_t)(n - 1)) - 1;
        seq_t geom = num2 / r_minus1;     /* integer division */

        /* S_p = 1 + factor1 * geom */
        seq_t Si = 1 + factor1 * geom;

        result *= Si;
    }

    return result;
}
