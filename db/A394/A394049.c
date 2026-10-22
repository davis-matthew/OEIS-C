/*
 * A394049  Smallest number k that is neither squarefree nor squareful such
 *          that rad(k) | s(n), where s = A120944 is the sequence of
 *          squarefree composite numbers and rad = A007947.
 * https://oeis.org/A394049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A020639(A120944(n))^2 * A119288(A120944(n)) where A020639
 *            = smallest prime factor, A120944 = the n-th square‑free
 *            composite, A119288 = the second smallest prime factor of that
 *            number
 * Range:     u8 n <= 16, u16 n <= 797, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
/* seq_t and idx_t are provided by the framework */

/*--- global state -----------------------------------------------------*/

static idx_t *spf = NULL;        /* smallest prime factor array */
static bool *sqfree = NULL;      /* true if the index is square‑free */
static seq_t *s_list = NULL;     /* array of square‑free composites */
static size_t s_count = 0;       /* number of elements in s_list          */
static size_t s_cap = 0;         /* current allocated capacity of s_list  */
static idx_t  cur_limit = 0;     /* highest number processed by the sieve */

/*--- helper functions -------------------------------------------------*/

/* ensure that s_list can hold at least (n+s_cap-1)/s_cap elements */
static void resize_s_list(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(s_list, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failure */
    s_list = tmp;
    s_cap = new_cap;
}

/* append a new value to the list of square‑free composites */
static void push_s_list(idx_t val)
{
    if (s_count == s_cap) resize_s_list(s_cap ? s_cap << 1 : 16);
    s_list[s_count++] = (seq_t)val;
}

/* rebuild the sieve up to at least 'limit' (limit > cur_limit) */
static void recompute_sieve(idx_t limit)
{
    /* allocate new arrays, with C++ casts */
    idx_t *new_spf    = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    bool *new_sqfree  = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_spf || !new_sqfree) abort();

    for (idx_t i = 0; i <= limit; ++i) new_sqfree[i] = true;

    /* Sieve for smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!new_spf[i]) new_spf[i] = i;   /* i is prime */
        if (i <= limit / i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (!new_spf[j]) new_spf[j] = i;
            }
        }
    }

    /* Mark non–square‑free numbers */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_spf[p] == p) {            /* p is prime */
            for (idx_t j = p * p; j <= limit; j += p * p) {
                new_sqfree[j] = false;
            }
        }
    }

    /* Append new square‑free composites to the list */
    for (idx_t i = cur_limit + 1; i <= limit; ++i) {
        if (i > 1 && new_spf[i] != i && new_sqfree[i]) {
            push_s_list(i);
        }
    }

    /* free old arrays and store new ones */
    free(spf);
    free(sqfree);
    spf = new_spf;
    sqfree = new_sqfree;
    cur_limit = limit;
}

/* make sure we have at least n terms in s_list */
static void ensure_n_terms(idx_t n)
{
    while (s_count < n) {
        idx_t target = cur_limit ? cur_limit * 2 : 2;
        recompute_sieve(target);
    }
}

/*---------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* n < 1 is undefined */

    ensure_n_terms(n);

    idx_t m   = (idx_t)s_list[n - 1];    /* the n-th square‑free composite */
    idx_t p1  = spf[m];                  /* smallest prime factor */
    idx_t rest = m / p1;
    idx_t p2  = spf[rest];                /* second smallest prime factor */

    /* Compute p1^2 * p2 as big integer product */
    seq_t result = (seq_t)p1;
    result *= (seq_t)p1;
    result *= (seq_t)p2;
    return result;
}
