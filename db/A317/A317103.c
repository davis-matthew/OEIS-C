/*
 * A317103  Expansion of e.g.f. -LambertW(-x) * Product_{k>=1} 1/(1-x^k).
 * https://oeis.org/A317103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A317103: a(n) = coefficient of x^n in -LambertW(-x) *
 *                 Product_{k>=1} 1/(1-x^k)                    */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Static helper arrays shared across calls */
static seq_t *p_arr = NULL;   /* partition numbers p[0..N]           */
static seq_t *fact_arr = NULL;/* factorials fact[0..N]               */
static seq_t *pow_arr = NULL; /* k^(k-1) values for k=1..N           */
static seq_t *a_arr = NULL;   /* sequence values a[0..N]             */
static idx_t cur_n = 0;        /* largest n already computed          */

/* Allocate / recompute all auxiliary tables up to n (inclusive) */
static void ensure_up_to(idx_t n)
{
    if (n <= cur_n) return;                       /* already computed */

    /* Free old memory if it exists */
    mem_free(p_arr);   mem_free(fact_arr); mem_free(pow_arr); mem_free(a_arr);

    /* Allocate new arrays (n+1 to include index 0) */
    p_arr   = mem_new(seq_t, (n + 1));
    fact_arr= mem_new(seq_t, (n + 1));
    pow_arr = mem_new(seq_t, (n + 1));
    a_arr   = mem_new(seq_t, (n + 1));
    if (!p_arr || !fact_arr || !pow_arr || !a_arr) return; /* OOM – caller gets 0 */

    /* ---- 1. Partition numbers via DP ---- */
    for (idx_t i = 0; i <= n; ++i) p_arr[i] = 0;   /* clear */
    p_arr[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t m = k; m <= n; ++m) {
            p_arr[m] = p_arr[m] + p_arr[m - k];
        }
    }

    /* ---- 2. Factorials ---- */
    fact_arr[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact_arr[i] = seq_mul(fact_arr[i - 1], (seq_t)i);
    }

    /* ---- 3. Powers k^(k-1) ---- */
    pow_arr[0] = 1;   /* unused */
    for (idx_t i = 1; i <= n; ++i) {
        pow_arr[i] = ipow((seq_t)i, (seq_t)(i - 1));
    }

    /* ---- 4. Sequence a(n) ---- */
    a_arr[0] = 0;
    for (idx_t nn = 1; nn <= n; ++nn) {
        seq_t result = 0;
        seq_t prod   = 1;    /* product of integers (k+1)...nn (empty prod = 1) */
        for (idx_t k = nn; ; --k) {
            seq_t term = prod;                            /* (n! / k!) part    */
            term = seq_mul(term, pow_arr[k]);             /* k^(k-1)            */
            term = seq_mul(term, p_arr[nn - k]);          /* * p(n-k)           */
            result = result + term;                       /* accumulation       */
            if (k == 1) break;
            prod = seq_mul(prod, (seq_t)k);               /* extend product     */
        }
        a_arr[nn] = result;
    }

    cur_n = n;
}

/* External entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_up_to(n);
    return a_arr[n];
}
