/*
 * A390889  a(n) = Sum_{k=0..n} 2^k * 5^(n-k) * Stirling2(n,k).
 * https://oeis.org/A390889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * 5^(n-k) * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(0) = 1; a(n) = 2 * Sum_{j=0}^{n-1} 5^j * C(n-1,j) * a(n-1-j) */

#include <stdlib.h>
#include <stddef.h>

/*--- internal cache for a(n) values ----------------------------------------*/
static seq_t *a_cache = NULL;   /* allocated array of a(0)...a(N) */
static size_t a_cap  = 0;        /* current capacity of a_cache    */
static size_t a_len  = 0;        /* number of values actually stored */

/*--- cache for powers of 5: pow5[i] = 5^i ---------------------------------*/
static seq_t *pow5_cache = NULL;
static size_t pow5_cap  = 0;
static size_t pow5_len  = 0;

/*--- small helper: enlarge a_cache so it can hold indices 0..needed ----------------*/
static void ensure_a_cap(size_t needed)
{
    if (a_cap < needed) {
        size_t new_cap = (a_cap == 0) ? 16 : a_cap << 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

/*--- small helper: enlarge pow5_cache so it can hold powers 0..needed ------------
   (needed is an idx_t but used as size_t; we assume n fits in size_t) */
static void ensure_pow5_cap(size_t needed)
{
    if (pow5_cap < needed) {
        size_t new_cap = (pow5_cap == 0) ? 16 : pow5_cap << 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(pow5_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        pow5_cache = tmp;
        pow5_cap   = new_cap;
    }
}

/*--- expand pow5_cache up to exponent n (inclusive) ---------------------------*/
static void grow_pow5(idx_t n)
{
    /* if never initialized, set pow5_cache[0] = 1 */
    if (pow5_len == 0) {
        ensure_pow5_cap(1);
        pow5_cache[0] = (seq_t)1;
        pow5_len = 1;
    }
    /* enlarge storage if necessary */
    if (pow5_len <= (size_t)n)
        ensure_pow5_cap((size_t)n + 1);
    /* compute remaining powers */
    while (pow5_len <= (size_t)n) {
        pow5_cache[pow5_len] = pow5_cache[pow5_len - 1] * (seq_t)5;
        ++pow5_len;
    }
}

/*--- main sequence function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    /* If already computed, return it immediately. */
    if (n < a_len)
        return a_cache[n];

    /* Allocate space for all needed a values. */
    ensure_a_cap((size_t)n + 1);

    /* Ensure pow5 powers up to n are available. */
    grow_pow5(n);

    /* Compute the missing values up to n, filling the cache. */
    for (idx_t curr = a_len; curr <= n; ++curr) {
        if (curr == 0) {            /* a(0) = 1 */
            a_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;    /* C(curr‑1,0) */

        /* j runs from 0 to curr‑1 */
        for (idx_t j = 0; j < curr; ++j) {
            if (j > 0) {
                /* C(curr‑1, j) = C(curr‑1, j‑1) * (curr‑j) / j */
                binom = (binom * (seq_t)(curr - j)) / (seq_t)j;
            }
            /* term = 5^j * C(curr‑1, j) * a(curr‑1‑j) */
            seq_t term = pow5_cache[j] * binom * a_cache[curr - 1 - j];
            sum += term;
        }
        a_cache[curr] = sum * (seq_t)2;
    }

    a_len = (size_t)n + 1;
    return a_cache[n];
}
