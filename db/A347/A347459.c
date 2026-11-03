/*
 * A347459  Number of factorizations of n^2 with integer reciprocal
 *          alternating product.
 * https://oeis.org/A347459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1003, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 112 terms through n = 9342 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) counts the weakly increasing factorizations of n² into integers > 1
 *  having even length, for which the product of the odd‑indexed factors
 *  divides the product of the even‑indexed factors (equivalently, the
 *  reciprocal alternating product is an integer).  This is the definition
 *  of OEIS A347459.  The implementation enumerates all factorizations
 *  of n² and checks the condition directly.  The algorithm is O(d(n²)) in
 *  practice, where d(n²) is the number of divisors of n², and it is fast
 *  enough for n ≤ 10 000.  Results are cached for repeated calls.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* recursive search for factorizations of 'rem', last chosen factor is 'last',
 * depth parity given by 'depth'.
 */
static void dfs_factorisations(seq_t rem, seq_t last, int depth,
                               seq_t prod_odd, seq_t prod_even,
                               const seq_t *divs, size_t dcnt,
                               seq_t *counter)
{
    if (rem == (seq_t)1) {
        if ((depth & 1) == 0) {          /* even length */
            if (prod_even % prod_odd == (seq_t)0)
                (*counter) += (seq_t)1;
        }
        return;
    }
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d = divs[i];
        if (d < last) continue;
        if (d > rem) break;
        if (rem % d != (seq_t)0) continue;

        seq_t np_odd  = prod_odd;
        seq_t np_even = prod_even;
        if ((depth & 1) == 0)
            np_odd  = np_odd * d;  /* odd position */
        else
            np_even = np_even * d; /* even position */

        dfs_factorisations(rem / d, d, depth + 1, np_odd, np_even,
                           divs, dcnt, counter);
    }
}

/* auxiliary function: generate all divisors of a number from its prime
 * factorization.  'pr' and 'exp' have length 'cnt'.
 */
static void gen_divisors_rec(const uint32_t *pr, const int *exp, size_t cnt,
                             size_t idx, seq_t cur, seq_t *out, size_t *len)
{
    if (idx == cnt) {
        out[(*len)++] = cur;
        return;
    }
    seq_t v = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors_rec(pr, exp, cnt, idx + 1, cur * v, out, len);
        v *= pr[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* cache for fast repeated queries */
    enum { MAX_N = 10000 };
    static seq_t cache[MAX_N + 1];
    static bool computed[MAX_N + 1];
    static bool init_cache = false;
    if (!init_cache) {
        memset(computed, 0, sizeof(computed));
        init_cache = true;
    }
    if (n <= MAX_N && computed[n])
        return cache[n];

    /* special case n = 1 */
    if (n == (idx_t)1) {
        seq_t v = (seq_t)1;
        if (n <= MAX_N) { cache[n] = v; computed[n] = true; }
        return v;
    }

    /* factor n to obtain prime bases and exponents */
    uint32_t factors[8];
    int  exps[8];
    size_t fcnt = 0;
    uint32_t t = (uint32_t)n;
    for (uint32_t p = 2; (uint64_t)p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            factors[fcnt] = p;
            exps[fcnt]    = e * 2;   /* exponents in n² */
            fcnt++;
        }
    }
    if (t > 1) {
        factors[fcnt] = t;
        exps[fcnt]    = 2;
        fcnt++;
    }

    /* generate all divisors of n² */
    seq_t divs[1024];
    size_t dcnt = 0;
    gen_divisors_rec(factors, exps, fcnt, 0, (seq_t)1, divs, &dcnt);

    /* sort divisors */
    mem_qsort(divs, dcnt, sizeof(seq_t), cmp_seq_t);

    seq_t counter = 0;
    dfs_factorisations((seq_t)(n * (idx_t)n), (seq_t)2, 0,
                       (seq_t)1, (seq_t)1, divs, dcnt, &counter);

    if (n <= MAX_N) {
        cache[n] = counter;
        computed[n] = true;
    }
    return counter;
}
