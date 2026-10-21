/*
 * A280716  Expansion of Product_{k>=2} (1 + mu(2*k-1)^2*x^(2*k-1)), where
 *          mu() is the Moebius function (A008683).
 * https://oeis.org/A280716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct odd squarefree
 *            parts > 1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n); /* declare prototype for completeness */

/* Static storage for the DP table */
static seq_t *dp_cache = NULL;   /* dp_cache[i] = a(i) for 0 <= i < dp_cap */
static size_t dp_cap = 0;

/* ------------------------------------------------------------------------- */
static void recompute(idx_t max_n)
/* Build the DP table up to max_n (inclusive).  This routine is called
   only when a call to a(n) arrives with n >= dp_cap.  */
{
    /* --------------------------------------------------------------------- */
    /* 1. Build a smallest‑prime‑factor table up to max_n. */
    idx_t *spf = (idx_t *)calloc((size_t)max_n + 1, sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);              /* allocation failure */
    for (size_t i = 2; i <= (size_t)max_n; ++i) spf[i] = (idx_t)i;
    for (size_t i = 2; (size_t)i * (size_t)i <= (size_t)max_n; ++i) {
        if (spf[i] != (idx_t)i) continue;      /* not prime */
        for (size_t j = (size_t)i * (size_t)i; j <= (size_t)max_n; j += i) {
            if (spf[j] == (idx_t)j) spf[j] = (idx_t)i;
        }
    }

    /* --------------------------------------------------------------------- */
    /* 2. Collect all odd square‑free numbers > 1 up to max_n. */
    size_t parts_cap = (size_t)max_n / 2 + 2;   /* over‑estimate capacity */
    size_t *parts   = (size_t *)malloc(parts_cap * sizeof(size_t));
    if (!parts) { free(spf); exit(EXIT_FAILURE); }
    size_t parts_len = 0;

    for (idx_t p = 3; p <= max_n; p += 2) {
        /* Check square‑freeness using the spf table. */
        idx_t tmp = p;
        idx_t last = 0;
        bool squarefree = true;
        while (tmp > 1) {
            idx_t f = spf[(size_t)tmp];
            if (f == last) { squarefree = false; break; }
            last = f;
            tmp /= f;
        }
        if (squarefree) {
            if (parts_len == parts_cap) {
                parts_cap <<= 1;
                size_t *tmpptr = (size_t *)realloc(parts, parts_cap * sizeof(size_t));
                if (!tmpptr) { free(spf); free(parts); exit(EXIT_FAILURE); }
                parts = tmpptr;
            }
            parts[parts_len++] = (size_t)p;
        }
    }
    free(spf);

    /* --------------------------------------------------------------------- */
    /* 3. DP over distinct parts. */
    seq_t *newdp = (seq_t *)calloc((size_t)max_n + 1, sizeof(seq_t));
    if (!newdp) { free(parts); exit(EXIT_FAILURE); }
    newdp[0] = 1;   /* empty partition */

    for (size_t i = 0; i < parts_len; ++i) {
        size_t part = parts[i];
        for (size_t s = (size_t)max_n; s >= part; --s) {
            newdp[s] += newdp[s - part];
        }
    }

    free(parts);

    /* --------------------------------------------------------------------- */
    /* 4. Replace the old DP cache. */
    if (dp_cache) free(dp_cache);
    dp_cache = newdp;
    dp_cap   = (size_t)max_n + 1;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if ((size_t)n >= dp_cap) {
        recompute(n);
    }
    return dp_cache[(size_t)n];
}
