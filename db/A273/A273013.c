/*
 * A273013  Number of different arrangements of nonnegative integers on a pair
 *          of n-sided dice such that the dice can add to any integer from 0
 *          to n^2-1.
 * https://oeis.org/A273013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σk≥1 b_k² + Σk≥1 b_k·b_{k+1}, where b_k is the number of
 *            ordered factorizations of n into k factors >1
 * Range:     u8 n <= 35, u16 n <= 239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Generate all divisors >1 of a number given its prime factorisation.
   The result is stored in divs and dcnt is updated.
   The function uses recursion and assumes that divs has enough space. */
static void gen_divs_rec(const uint64_t *pr, const uint64_t *exp, size_t cnt,
                         idx_t cur, idx_t *divs, size_t *dcnt)
{
    if (cnt == 0) {
        if (cur > 1) {
            divs[(*dcnt)++] = cur;
        }
        return;
    }

    /* case: current prime not used */
    gen_divs_rec(pr, exp, cnt - 1, cur, divs, dcnt);

    /* case: current prime used with exponent from 1 to exp[cnt-1] */
    idx_t mult = cur;
    for (unsigned int i = 0; i < exp[cnt - 1]; ++i) {
        mult *= pr[cnt - 1];
        gen_divs_rec(pr, exp, cnt - 1, mult, divs, dcnt);
    }
}

/* Recursive enumeration of ordered factorizations of rem.
   depth = number of factors already chosen.
   b[depth+1] is incremented when one more factor completes a factorization. */
static void dfs(idx_t rem, int depth, seq_t *b, idx_t maxk)
{
    /* factor the current remainder */
    uint64_t pr[20], exp[20];
    size_t cnt = 0;
    factor_u64(rem, pr, exp, &cnt);

    /* generate all divisors >1 of rem */
    idx_t divs[512];
    size_t dcnt = 0;
    gen_divs_rec(pr, exp, cnt, (idx_t)1, divs, &dcnt);

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        idx_t next = rem / d;
        if (next == 1) {
            /* finished a factorization of length depth+1 */
            b[depth + 1] += (seq_t)1;
        } else {
            dfs(next, depth + 1, b, maxk);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    /* Determine the maximum possible length of a factorization:
       it equals the sum of exponents in the prime factorisation of n. */
    uint64_t pr[20], exp[20];
    size_t cnt = 0;
    factor_u64(n, pr, exp, &cnt);

    idx_t maxk = 0;
    for (size_t i = 0; i < cnt; ++i) {
        maxk += exp[i];
    }

    /* b[0] is unused; indices 1..maxk hold the b_k values. */
    seq_t *b = (seq_t *)malloc((maxk + 1) * sizeof(seq_t));
    if (!b) {
        return (seq_t)0; /* allocation failure */
    }
    for (idx_t i = 0; i <= maxk; ++i) {
        b[i] = (seq_t)0;
    }

    /* Enumerate all factorisations of n */
    dfs(n, 0, b, maxk);

    /* Compute the required sum */
    seq_t res = (seq_t)0;
    for (idx_t k = 1; k <= maxk; ++k) {
        res += b[k] * b[k];
    }
    for (idx_t k = 1; k < maxk; ++k) {
        res += b[k] * b[k + 1];
    }

    free(b);
    return res;
}
