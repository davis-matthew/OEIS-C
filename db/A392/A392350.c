/*
 * A392350  a(n) = Sum_{k=0..floor(3*n/11)} binomial(3*n-10*k,k).
 * https://oeis.org/A392350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(3*n/11)} binomial(3*n-10*k,k)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 89, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) + a(n-4) - a(n-5) + a(n-6) - a(n-7) + a(n-8) - a(n-9) + a(n-10) */
/* Initial terms: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=3, a(5)=6, a(6)=9, a(7)=12, a(8)=21, a(9)=39 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;   /* capacity of allocated cache */
static size_t seq_used  = 0;   /* number of computed terms */

/* Ensure the cache holds at least n+1 entries and contains all terms up to n */
static void
ensure_seq(idx_t n)
{
    /* Allocate and initialize first 10 terms if needed */
    if (seq_cache == NULL) {
        seq_cap  = 10;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq_cache == NULL)
            exit(EXIT_FAILURE); /* out of memory */
        seq_cache[0] = 1; seq_cache[1] = 1; seq_cache[2] = 1; seq_cache[3] = 1;
        seq_cache[4] = 3; seq_cache[5] = 6; seq_cache[6] = 9; seq_cache[7] = 12;
        seq_cache[8] = 21; seq_cache[9] = 39;
        seq_used = 10;
    }

    /* Grow cache if required */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(EXIT_FAILURE);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Compute missing terms */
    if (n >= seq_used) {
        for (idx_t m = (idx_t)seq_used; m <= n; ++m) {
            /* Positive contributions */
            seq_t pos = (seq_cache[m-1] * 2)          /* 2*a(m-1) */
                      + (seq_cache[m-3] * 2)          /* 2*a(m-3) */
                      + seq_cache[m-4]                /*   a(m-4) */
                      + seq_cache[m-6]                /*   a(m-6) */
                      + seq_cache[m-8]                /*   a(m-8) */
                      + seq_cache[m-10];              /*   a(m-10) */

            /* Negative contributions */
            seq_t neg = (seq_cache[m-2] * 2)          /* 2*a(m-2) */
                      + seq_cache[m-5]                /*   a(m-5) */
                      + seq_cache[m-7]                /*   a(m-7) */
                      + seq_cache[m-9];               /*   a(m-9) */

            seq_cache[m] = pos - neg;                /* final term */
        }
        seq_used = (size_t)n + 1;
    }
}

seq_t
a(idx_t n)
{
    ensure_seq(n);
    return seq_cache[(size_t)n];
}
