/*
 * A284204  Eighth column of Euler's difference table in A068106.
 * https://oeis.org/A284204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n>=8: a(n) = Sum_{j=0}^{n-7} (-1)^j * binomial(n-7,j) *
 *            (n-j)!. For n=1..6: a(n)=0; a(7)=5040
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *fact = NULL;          /* factorial table, fact[i] = i!   */
static idx_t fact_len = 0;          /* highest index computed         */

/* ensure factorial table up to n is available */
static void ensure_factorial(idx_t n)
{
    if (n < fact_len)
        return;
    if (fact_len == 0) {
        fact = (seq_t *)malloc(sizeof(seq_t));
        fact[0] = (seq_t)1;
        fact_len = 0;
    }
    if (n > fact_len) {
        seq_t *tmp = (seq_t *)realloc(fact, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        fact = tmp;
        for (idx_t i = fact_len + 1; i <= n; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_len = n;
    }
}

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n <= 6) return (seq_t)0;
    if (n == 7) return (seq_t)5040;   /* 7! */

    idx_t m = n - 7;                  /* upper limit for j */
    ensure_factorial(n);              /* need fact[0..n] */

    seq_t pos = 0;                    /* sum of terms with even j */
    seq_t neg = 0;                    /* sum of terms with odd  j */

    for (idx_t j = 0; j <= m; ++j) {
        /* binomial(m, j) * fact[n-j] */
        seq_t term = binomial(m, j);
        term = term * fact[n - j];
        if (j & 1)
            neg += term;
        else
            pos += term;
    }
    return pos - neg;                 /* pos >= neg for all n */
}
