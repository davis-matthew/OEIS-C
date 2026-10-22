/*
 * A360557  Numbers > 1 whose sorted first differences of 0-prepended prime
 *          indices have non-integer median.
 * https://oeis.org/A360557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer > 1 whose sorted first differences of *
 *            0‑prepended prime indices have non‑integer median. *
 *            (Even‑length differences with middle two having opposite
 *            parity.) * * The implementation builds a dynamic sieve of
 *            smallest prime factors * (spf) and a prime‑index table
 *            (prime_index)....
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Sieve for smallest prime factor (spf) and prime indices.          */
/* ------------------------------------------------------------------ */

static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i */
static idx_t *prime_index = NULL;  /* 1‑based index of prime i, or 0 if composite */
static idx_t current_limit = 0;    /* highest number sieved */

static void ensure_limit(idx_t limit)
{
    if (current_limit >= limit)
        return;

    /* free previous tables if any */
    if (spf)          free(spf);
    if (prime_index)  free(prime_index);

    /* allocate new tables */
    size_t sz = (size_t)limit + 1;
    spf = (idx_t *)malloc(sz * sizeof(idx_t));
    prime_index = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!spf || !prime_index)  exit(3);

    /* initialise spf as itself */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = i;
    if (limit >= 0) spf[0] = 0;
    if (limit >= 1) spf[1] = 1;

    /* sieve for spf */
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (spf[i] == i) {               /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }

    /* fill prime_index table */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {               /* i is prime */
            ++cnt;
            prime_index[i] = cnt;        /* 1‑based index */
        }
    }

    current_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Property test for a single number.                                */
/* ------------------------------------------------------------------ */

static bool satisfies(idx_t m)
{
    if (m <= 1)
        return false;

    if (current_limit < m)
        ensure_limit(m);

    /* build list of prime indices with multiplicity */
    uint32_t indices[128];
    size_t k = 0;
    idx_t tmp = m;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        idx_t idxp = prime_index[p];
        while (tmp % p == 0) {
            if (k < 128)   /* safe for all n < 2^128 */
                indices[k++] = (uint32_t)idxp;
            tmp /= p;
        }
    }
    if (k == 0)
        return false;            /* should never happen for m > 1 */

    /* sort the indices (insertion sort, k is small) */
    for (size_t i = 1; i < k; ++i) {
        uint32_t key = indices[i];
        size_t j = i;
        while (j > 0 && indices[j-1] > key) {
            indices[j] = indices[j-1];
            --j;
        }
        indices[j] = key;
    }

    /* build first differences array (with leading 0) */
    uint32_t diffs[128];
    diffs[0] = indices[0];
    for (size_t i = 1; i < k; ++i)
        diffs[i] = indices[i] - indices[i-1];

    size_t dlen = k;                 /* number of differences = k */
    if (dlen % 2 == 1)
        return false;                /* odd length => integer median */

    /* sort differences */
    for (size_t i = 1; i < dlen; ++i) {
        uint32_t key = diffs[i];
        size_t j = i;
        while (j > 0 && diffs[j-1] > key) {
            diffs[j] = diffs[j-1];
            --j;
        }
        diffs[j] = key;
    }

    /* median is average of diffs[dlen/2-1] and diffs[dlen/2] */
    uint32_t a = diffs[dlen/2 - 1];
    uint32_t b = diffs[dlen/2];
    return ((a + b) & 1U) != 0;      /* non‑integer iff sum odd */
}

/* ------------------------------------------------------------------ */
/*  Store of discovered sequence terms.                               */
/* ------------------------------------------------------------------ */

static seq_t *terms = NULL;   /* array of qualifying numbers */
static size_t terms_cap = 0;   /* capacity of the array */
static size_t terms_len = 0;   /* number of elements stored */
static idx_t next_candidate = 2;  /* next integer to test */

/* ------------------------------------------------------------------ */
/*  Sequence function.                                                */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1 */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* grow sieve if needed */
        if (next_candidate > current_limit) {
            idx_t new_limit = next_candidate * 2;
            if (new_limit < current_limit + 1)
                new_limit = current_limit + 1;
            ensure_limit(new_limit);
        }

        if (satisfies(next_candidate)) {
            if (terms_len == terms_cap) {           /* reallocate */
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(4);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[n - 1];          /* 0‑based index */
}
