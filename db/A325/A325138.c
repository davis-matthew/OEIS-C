/*
 * A325138  a(n) = Sum_{k=0..n} Sum_{j=0..n-k} binomial(j+k, k)*|Stirling1(n,
 *          j+k)|*(k+1)^j.
 * https://oeis.org/A325138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{j=0..n-k} binomial(j+k,
 *            k)*|Stirling1(n, j+k)|*(k+1)^j. The implementation uses the
 *            identity a(n) = Sum_{m=0..n} S(n,m)*c_m with c_m = Sum_{k=0..m}
 *            binomial(m,k)*(k+1)^(m-k) and a recurrence derived from it:
 *            a(0)=1 a(n)=∑_{j=1..n} a(n-j) * C(n-1,j-1) * ( (j!·H_j)+
 *            (j-1)!...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/factorial.h"

/* Static caches for factorials, harmonic numerators, and a(n) values */
static seq_t *fact = NULL;
static idx_t fact_size = 0;

static seq_t *Hnum = NULL;   /* Hnum[j] = j! * H_j */
static idx_t Hnum_size = 0;

static seq_t *aval = NULL;   /* a array */
static idx_t aval_size = 0;

/* Ensure factorial array up to n */
static void ensure_fact(idx_t n) {
    if (fact_size > n) return;
    size_t newsize = (size_t)(n + 1);
    seq_t *p = (seq_t *)realloc(fact, newsize * sizeof(seq_t));
    if (!p) exit(1);
    fact = p;
    for (idx_t i = fact_size; i <= n; ++i) {
        fact[i] = factorial(i);
    }
    fact_size = n + 1;
}

/* Ensure harmonic numerator array up to n */
static void ensure_Hnum(idx_t n) {
    if (Hnum_size > n) return;
    size_t newsize = (size_t)(n + 1);
    seq_t *p = (seq_t *)realloc(Hnum, newsize * sizeof(seq_t));
    if (!p) exit(1);
    Hnum = p;
    if (Hnum_size == 0) {
        Hnum[0] = 0;
        Hnum_size = 1;
    }
    for (idx_t i = Hnum_size; i <= n; ++i) {
        seq_t term1 = (seq_t)i * Hnum[i - 1];
        seq_t term2 = fact[i - 1];
        Hnum[i] = term1 + term2;
    }
    Hnum_size = n + 1;
}

/* Ensure a(n) array up to n */
static void ensure_a(idx_t n) {
    if (aval_size > n) return;
    /* Make sure supporting tables are ready */
    ensure_fact(n);
    ensure_Hnum(n);
    size_t newsize = (size_t)(n + 1);
    seq_t *p = (seq_t *)realloc(aval, newsize * sizeof(seq_t));
    if (!p) exit(1);
    aval = p;
    if (aval_size == 0) {
        aval[0] = 1;   /* a(0)=1 */
        aval_size = 1;
    }

    for (idx_t i = aval_size; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom_val = 1;            /* binom(i-1,0) */
        for (idx_t j = 1; j <= i; ++j) {
            /* term = a[i-j] * C(i-1, j-1) */
            seq_t term = aval[i - j] * binom_val;
            /* temp = Hnum[j] + fact[j-1] = j! * H_j + (j-1)! */
            seq_t temp = Hnum[j] + fact[j - 1];
            term *= temp;
            sum += term;
            if (j < i) {
                /* update binom_val = binom(i-1, j) from binom(i-1, j-1) */
                binom_val *= (seq_t)(i - j); /* multiply by (i-j) */
                binom_val /= (seq_t)j;       /* divide by j */
            }
        }
        aval[i] = sum;
    }
    aval_size = n + 1;
}

/* Public function */
seq_t a(idx_t n) {
    ensure_a(n);
    return aval[n];
}
