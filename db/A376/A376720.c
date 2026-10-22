/*
 * A376720  Product of numbers m that are neither squarefree nor prime powers
 *          and rad(m), where rad = A007947.
 * https://oeis.org/A376720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 124, u32 n <= 9313, u64 n <= 9313,
 *            u128 n <= 9313, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376720:  a(n) = m * rad(m) where m is the n‑th integer that
   is not squarefree and not a prime power (has ≥2 distinct primes and
   at least one exponent ≥2).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External integer types supplied by the build environment. */
extern seq_t a(idx_t n);

/* --------------------- Smallest prime factor sieve --------------------- */
static int  *spf   = NULL;   /* spf[i] = smallest prime divisor of i, i>=2 */
static size_t spf_limit = 0; /* highest index currently sieved */

/* rebuild the sieve up to at least 'limit' (inclusive) */
static void init_spf(size_t limit)
{
    int *new_spf = (int *)malloc((limit + 1) * sizeof(int));
    if (!new_spf) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= limit; ++i) new_spf[i] = (int)i;
    new_spf[0] = 0;
    if (limit >= 1) new_spf[1] = 1;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_spf[i] == (int)i) {
            for (size_t j = i * i; j <= limit; j += i)
                if (new_spf[j] == (int)j) new_spf[j] = (int)i;
        }
    }

    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* ------------------------ Sequence cache ------------------------------ */
static seq_t *seq_vals = NULL; /* a(1), a(2), ... */
static size_t seq_cap  = 0;    /* array capacity */
static size_t seq_len  = 0;    /* number of terms stored */

static void seq_grow(void)
{
    size_t new_cap = seq_cap ? 2 * seq_cap : 16;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* Last integer examined when building the sequence. */
static idx_t checked_up_to = 0;

/* --------------------------- Main function ----------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, below that is irrelevant */

    /* extend precomputation until we have at least n terms */
    while (seq_len < n) {
        /* ensure the sieve covers the next candidate number */
        idx_t next_candidate = checked_up_to + 1;
        if ((size_t)next_candidate > spf_limit) {
            size_t new_lim = spf_limit ? spf_limit * 2 : 16;
            while (new_lim < (size_t)next_candidate) new_lim <<= 1;
            init_spf(new_lim);
        }

        ++checked_up_to;
        idx_t m = checked_up_to;
        int temp = (int)m;

        seq_t rad = (seq_t)1;   /* product of distinct primes */
        int distinct   = 0;
        bool has_dup   = false;
        int lastp = 0, exp = 0;

        while (temp > 1) {
            int p = spf[temp];
            if (p != lastp) {            /* new prime factor */
                ++distinct;
                rad = rad * (seq_t)p;
                exp = 1;
                lastp = p;
            } else {                    /* same prime continues */
                ++exp;
            }
            if (exp >= 2) has_dup = true;
            temp /= p;
        }

        if (distinct >= 2 && has_dup) { /* qualifies */
            if (seq_len == seq_cap) seq_grow();
            seq_vals[seq_len++] = rad * (seq_t)m;
        }
    }

    return seq_vals[n - 1];
}
