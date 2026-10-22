/*
 * A347571  Expansion of the e.g.f. (-1 - 2*x - 2*log(1 - x) + exp(-2*x) / (1
 *          - x)^2) / 4 + 1.
 * https://oeis.org/A347571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..⌊n/2⌋} ceil(2^(k-2)) · k! · S(n,k) where S(n,k)
 *            are Stirling numbers of the second kind. This implementation
 *            evaluates the equivalent closed form a(n) = ( Σ_{j=0}^{n} (-2)^j
 *            · (n!/j!) · (n-j+1) + 2·(n-1)! ) / 4 for n ≥ 2, with a(0)=1 and
 *            a(1)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The types seq_t and idx_t are supplied by the build system. */
static seq_t *fact    = NULL;  /* factorials */
static seq_t *powAbs  = NULL;  /* 2^j (positive) */
static seq_t *ratio   = NULL;  /* n! / j! */
static seq_t *a_seq   = NULL;  /* computed a(n) values */
static size_t   cap   = 0;     /* current allocation size */
static idx_t    last_n = 0;    /* highest n for which a_seq is ready */

/* Initialise the arrays on first use. */
static void init_arrays(void)
{
    if (fact != NULL) return;                    /* already initialised */
    cap = 16;                                    /* initial capacity */
    fact   = (seq_t *)malloc(cap * sizeof(seq_t));
    powAbs = (seq_t *)malloc(cap * sizeof(seq_t));
    ratio  = (seq_t *)malloc(cap * sizeof(seq_t));
    a_seq  = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!fact || !powAbs || !ratio || !a_seq) return; /* allocation failed */

    /* base case n = 0 */
    fact[0]  = (seq_t)1;
    powAbs[0] = (seq_t)1;
    ratio[0]  = (seq_t)1;    /* 0! / 0! = 1 */
    a_seq[0]  = (seq_t)1;    /* a(0) = 1 */
    last_n    = 0;
}

/* Make sure the arrays contain values up to n. */
static void ensure_capacity(idx_t n)
{
    init_arrays();

    if (n <= last_n) return;                      /* nothing to do */

    /* enlarge the allocation if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        fact   = (seq_t *)realloc(fact,   new_cap * sizeof(seq_t));
        powAbs = (seq_t *)realloc(powAbs, new_cap * sizeof(seq_t));
        ratio  = (seq_t *)realloc(ratio,  new_cap * sizeof(seq_t));
        a_seq  = (seq_t *)realloc(a_seq,  new_cap * sizeof(seq_t));
        if (!fact || !powAbs || !ratio || !a_seq) return; /* failure */
        cap = new_cap;
    }

    /* Build successive rows */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        /* factorial */
        fact[k] = fact[k - 1] * (seq_t)k;
        /* power of two */
        powAbs[k] = powAbs[k - 1] * (seq_t)2;

        /* update ratio: new ratio[j] = k * old_ratio[j] for j<k,
           and ratio[k] = 1 (k! / k! = 1) */
        for (idx_t j = 0; j < k; ++j) {
            ratio[j] = ratio[j] * (seq_t)k;
        }
        ratio[k] = (seq_t)1;

        /* compute the alternating sum */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t j = 0; j <= k; ++j) {
            seq_t term = ratio[j];
            term = term * powAbs[j];
            term = term * (seq_t)(k - j + 1);
            if ((j & 1) == 0) {
                pos += term;
            } else {
                neg += term;
            }
        }
        seq_t sum = pos - neg;          /* guaranteed non‑negative */

        /* a(k) = (sum + 2*(k-1)!)/4 for k ≥ 2;
           for k = 1 this expression already yields 0. */
        seq_t tmp = sum + (fact[k - 1] << 1);
        a_seq[k]  = tmp / (seq_t)4;
    }

    last_n = n;
}

/* Public function required by the interface. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n == 0) return (seq_t)1; /* a(0) = 1, already stored */
    return a_seq[n];
}
