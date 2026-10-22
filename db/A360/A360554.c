/*
 * A360554  Numbers > 1 whose unordered prime signature has non-integer
 *          median.
 * https://oeis.org/A360554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer >1 whose unordered prime signature has
 *            non-integer median
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   sort_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sort_int.h"

/* Sieve smallest prime factor up to limit */
static void build_spf(uint32_t **spf, int *limit)
{
    int lim = *limit;
    *spf = (uint32_t*)malloc((size_t)(lim + 1) * sizeof(uint32_t));
    for (int i = 0; i <= lim; ++i) (*spf)[i] = 0;
    for (int i = 2; i <= lim; ++i) {
        if ((*spf)[i] == 0) {
            (*spf)[i] = i;
            if ((int64_t)i * i <= lim)
                for (int j = i * i; j <= lim; j += i)
                    if ((*spf)[j] == 0) (*spf)[j] = i;
        }
    }
    *limit = lim;
}

/* Global state for memoized sequence */
static seq_t *terms = NULL;       /* cached terms */
static size_t terms_cap = 0;
static size_t terms_len = 0;

static idx_t next_candidate = 2;   /* next integer to test */
static uint32_t *spf = NULL;       /* smallest prime factor array */
static int spf_limit = 0;          /* current limit of spf array */

/* Ensure we have computed at least n terms */
static void ensure_terms(idx_t n)
{
    /* Ensure spf array exists */
    if (spf == NULL) {
        build_spf(&spf, &spf_limit);
    }

    /* Grow terms array if needed */
    if (terms_cap == 0) {
        terms_cap = 1024;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < (size_t)n) {
        if (next_candidate > spf_limit) {
            /* Extend spf limit */
            int new_limit = (int)((uintptr_t)next_candidate * 2);
            free(spf);
            build_spf(&spf, &new_limit);
        }

        uint64_t x = next_candidate;
        /* factorization */
        int exps[64];
        int k = 0;
        while (x > 1) {
            uint32_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            exps[k++] = cnt;
        }

        bool qualify = false;
        if (k % 2 == 0) {
            sort_int(exps, k);
            int a = exps[k / 2 - 1];
            int b = exps[k / 2];
            if ((a & 1) != (b & 1)) qualify = true;
        }

        if (qualify) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n==0 but not used */
    ensure_terms(n);
    return terms[n - 1];
}
