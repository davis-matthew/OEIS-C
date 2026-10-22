/*
 * A397221  Numbers m such that rad(m) * omega(m) = m.
 * https://oeis.org/A397221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397221: numbers m such that rad(m) * omega(m) = m
   (computed by a simple sieve up to a sufficient bound) */

#include <stdint.h>
#include <stdlib.h>

/* The static state that holds the pre‑computed terms. */
static seq_t *seq  = NULL;   /* 0‑based array of terms */
static idx_t seq_len = 0;    /* number of terms stored */

/* ------------------------------------------------------------------ */
/* Build a table of all terms <= limit.  The result array contains
   exactly those terms, sorted increasingly.  The caller receives the
   pointer in *out and its length in *len.  All temporary arrays are
   freed before return.  -------------------------------------------------
   This routine must be small and fast; the limit is only a few
   hundred thousand, so the memory usage stays modest. */
static void compute_terms (idx_t limit, seq_t **out, size_t *len)
{
    size_t L = (size_t)(limit + 1);

    seq_t  *rad    = (seq_t *)calloc(L, sizeof(seq_t));
    unsigned int *omega = (unsigned int *)calloc(L, sizeof(unsigned int));
    unsigned int *spf  = (unsigned int *)calloc(L, sizeof(unsigned int));
    uint32_t *primes = (uint32_t *)malloc(L * sizeof(uint32_t));
    size_t prime_cnt = 0;

    /* Linear sieve to fill rad, omega and spf. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = (unsigned int)i;
            primes[prime_cnt++] = (uint32_t)i;
            rad[i]   = (seq_t)i;
            omega[i] = 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p   = primes[j];
            idx_t    x   = i * p;
            if ((idx_t)x > limit) break;
            spf[x] = p;
            if ((i % p) == 0) {                /* p divides i */
                rad[x]   = rad[i];
                omega[x] = omega[i];
                break;
            } else {                           /* p is new prime factor */
                rad[x]   = rad[i] * (seq_t)p;
                omega[x] = omega[i] + 1;
            }
        }
    }

    /* Gather the terms satisfying rad(m)*omega(m) == m. */
    seq_t *tmp  = (seq_t *)malloc(L * sizeof(seq_t));
    size_t count = 0;
    for (idx_t m = 2; m <= limit; ++m) {
        if ((rad[m] * (seq_t)omega[m]) == (seq_t)m) {
            tmp[count++] = (seq_t)m;
        }
    }

    *out = tmp;
    *len = count;

    free(rad);
    free(omega);
    free(spf);
    free(primes);
}

/* ------------------------------------------------------------------ */
/* Ensure that seq contains at least n terms.  If not, recompute
   with a larger bound.  The state is updated in place. */
static void ensure_sequence (idx_t n)
{
    if (seq != NULL) {
        if (seq_len >= n) return;
        free(seq);          /* discard old table */
        seq = NULL;
    }

    idx_t bound = 500000;    /* initial bound – enough for 10k+ terms */
    while (1) {
        seq_t *tmp;
        size_t len;
        compute_terms(bound, &tmp, &len);
        if (len >= n) {
            seq = tmp;
            seq_len = (idx_t)len;
            break;
        }
        free(tmp);
        bound <<= 1;        /* double the bound and try again */
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function.  Returns a(n) for n >= 1.  For n < 1
   a value of 0 is returned, which is effectively an error value. */
seq_t a (idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_sequence(n);
    return seq[n - 1];
}
