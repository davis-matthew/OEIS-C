/*
 * A369210  Numbers k such that the number of divisors of k^2 is a power of 3.
 * https://oeis.org/A369210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_three_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the number of divisors of k^2 is a power of 3.
   The function a(n) returns the n-th term (1‑based index). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_power_of_three_seq.h"

/* --------- backing data structures --------- */

/* cache for the sequence terms */
static seq_t *seq_cache = NULL;   /* sequence array */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* capacity of seq_cache */

/* cache for smallest prime factor */
static uint32_t *spf = NULL;      /* array of smallest prime factors */
static idx_t    spf_cap = 0;      /* capacity of spf array */

/* last integer that was examined (used to continue scanning k). */
static idx_t last_k = 0;

/* -------------------------------------------------------------- */

/* expand spf array up to limit (inclusive) using a simple sieve */
static void init_spf(idx_t limit)
{
    if (limit <= spf_cap) return;          /* already large enough */

    uint32_t *new_spf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();                 /* out of memory */

    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {              /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }
    new_spf[1] = 1;                         /* convention */

    free(spf);
    spf = new_spf;
    spf_cap = limit;
}

/* append a new term to the sequence cache */
static void push_seq_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *new_mem = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_mem) abort();
        seq_cache = new_mem;
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* -------------------------------------------------------------- */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* initialise first term on first call */
    if (seq_len == 0) {
        push_seq_term((seq_t)1);    /* a(1) = 1 */
        last_k = 1;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        ++last_k;                     /* next integer to examine */

        /* ensure spf data is available */
        init_spf(last_k);

        /* factor last_k using spf and compute divisor count of k^2 */
        idx_t temp = last_k;
        seq_t d = 1;
        while (temp > 1) {
            uint32_t p = spf[temp];
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            d *= (seq_t)(2 * e + 1);
        }

        if (is_power_of_three_seq(d))
            push_seq_term((seq_t)last_k);
    }

    return seq_cache[n - 1];
}
