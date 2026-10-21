/*
 * A278847  a(n) = permanent M_n where M_n is the n X n matrix m(i,j) = i^2 +
 *          j^2.
 * https://oeis.org/A278847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n×n matrix M with entries M_{i,j}=i²+j².
 *            For a matrix of the form f(i)+g(j), its permanent equals
 *            Σ_{k=0}^n e_k(f)·e_{n−k}(g)·k!·(n−k)!, where e_k are elementary
 *            symmetric sums of f and g. Here f(i)=g(i)=i², so e_k(f)=e_k(g)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *e   = NULL;   /* elementary symmetric sums of i² up to current n */
static seq_t *fact;        /* factorials k! up to current n */
static idx_t cur_n = 0;    /* largest n processed so far */
static int initialized = 0;

/* Ensure e[0..n] and fact[0..n] are computed. */
static void init_arrays(idx_t n)
{
    if (!initialized) {                                /* first call */
        e   = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        fact= (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!e || !fact) return;
        e[0]    = (seq_t)1;
        fact[0] = (seq_t)1;
        cur_n   = 0;
        initialized = 1;
    } else if (n > cur_n) {                            /* extend to larger n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *e_new    = (seq_t*)realloc(e,    new_cap * sizeof(seq_t));
        seq_t *fact_new = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
        if (!e_new || !fact_new) return;
        e    = e_new;
        fact = fact_new;

        /* initialise new entries */
        for (idx_t i = cur_n + 1; i <= n; ++i) {
            e[i]    = (seq_t)0;
            fact[i] = (seq_t)0;
        }

        /* update elementary symmetric sums and factorials */
        for (idx_t i = cur_n + 1; i <= n; ++i) {
            seq_t a = (seq_t)i * (seq_t)i;          /* i² */
            for (idx_t j = i; j > 0; --j) {
                e[j] += a * e[j - 1];
            }
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        cur_n = n;
    }
}

seq_t a(idx_t n)
{
    init_arrays(n);
    if (!initialized) return (seq_t)0;          /* allocation failure guard */

    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = e[k];
        term       = term * e[n - k];
        term       = term * fact[k];
        term       = term * fact[n - k];
        result     = result + term;
    }
    return result;
}
