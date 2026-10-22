/*
 * A359777  Numbers k such that A356163(k) = 1 but A359774(k) = 0, where
 *          A359774 is the parity of Dirichlet inverse of the former (which is
 *          the characteristic function of the numbers with an even sum of
 *          prime factors, with repetition).
 * https://oeis.org/A359777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359777:  k such that A356163(k)=1 (sum of prime factors with multiplicity is even)
 * and A359774(k)=0 (parity of the Dirichlet inverse of that characteristic function).
 * This implementation precomputes f(k)=1 if the prime‑factor sum is even, otherwise 0,
 * and then computes the parity of its Dirichlet inverse g(k) using the recurrence
 *   g(1) = 1
 *   g(n) = f(n) XOR Σ_{d|n, 2≤d<n} f(d) & g(n/d)    for n>1
 *  Numbers with f(k)=1 and g(k)=0 are collected in the sequence.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state */
static idx_t *spf      = NULL;   /* smallest prime factor for each n */
static seq_t *fpar     = NULL;   /* f(n): 1 if sum of prime factors even, else 0 */
static seq_t *gpar     = NULL;   /* parity of Dirichlet inverse of f */
static seq_t *smod     = NULL;   /* parity of sum of prime factors */
static idx_t  maxSize  = 0;      /* maximum n for which tables are valid */

static seq_t *terms     = NULL;  /* stored sequence terms */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* capacity of terms array */

static void init_tables(idx_t limit)
{
    if (limit <= maxSize)
        return;

    /* allocate new tables */
    idx_t *new_spf  = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    seq_t *new_fpar = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    seq_t *new_gpar = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    seq_t *new_smod = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!new_spf || !new_fpar || !new_gpar || !new_smod)
        abort();

    /* Sieve for smallest prime factor */
    for (idx_t i = 0; i <= limit; ++i)
        new_spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_spf[i] == i) {                 /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == j)
                    new_spf[j] = i;
            }
        }
    }

    /* base values */
    new_smod[1] = 0;                /* sum of prime factors of 1 is 0 (even) */
    new_fpar[1] = 1;                 /* characteristic of even-sum numbers */
    new_gpar[1] = 1;                 /* Dirichlet inverse at 1 equals 1 */

    /* compute parity of sum of prime factors and f(n) */
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t p   = new_spf[i];
        idx_t m   = i / p;
        new_smod[i] = new_smod[m] ^ (seq_t)(p & 1);
        new_fpar[i] = (new_smod[i] == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* compute g(n) iteratively using the recurrence */
    for (idx_t n = 2; n <= limit; ++n) {
        seq_t parity = new_fpar[n];          /* start with f(n) */
        for (idx_t d = 2; d * d <= n; ++d) {
            if (n % d == 0) {
                idx_t d1 = d;               /* divisor d1 > 1 */
                idx_t d2 = n / d;           /* counterpart divisor d2 > 1 */
                parity ^= (new_fpar[d1] & new_gpar[d2]);
                if (d1 != d2)
                    parity ^= (new_fpar[d2] & new_gpar[d1]);
            }
        }
        new_gpar[n] = parity;
    }

    /* replace old tables */
    free(spf);
    free(fpar);
    free(gpar);
    free(smod);
    spf   = new_spf;
    fpar  = new_fpar;
    gpar  = new_gpar;
    smod  = new_smod;
    maxSize = limit;
}

static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 8;
        seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr)
            abort();
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static idx_t last_k = 0;   /* largest k examined so far */

    if (n == 0)   /* undefined for n <= 0, but return 0 for safety */
        return 0;

    /* Ensure tables cover a reasonable upper bound first */
    if (maxSize < 200000) {
        init_tables(200000);
    }

    /* Generate terms until we have the n-th */
    while (terms_len < n) {
        if (last_k + 1 > maxSize)
            init_tables(last_k + 1);   /* extend if needed */

        ++last_k;
        if (fpar[last_k] == (seq_t)1 && gpar[last_k] == (seq_t)0)
            add_term((seq_t)last_k);
    }

    return terms[n - 1];
}
