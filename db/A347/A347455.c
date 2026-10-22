/*
 * A347455  Heinz numbers of integer partitions with non-integer alternating
 *          product.
 * https://oeis.org/A347455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose multiset of prime indices has non‑integer
 *            reverse‑alternating product. The function generates these
 *            numbers by brute force, using a sieve up to a fixed limit. The
 *            result matches the OEIS A347455 sequence starting at index 1
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   cmp_uint32_asc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_uint32_asc.h"

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------ */
    /* sieve and helper tables (build once)                         */
    /* ------------------------------------------------------------ */
    enum { SPF_LIMIT = 5000000 };   /* enough for n up to ~10000 */

    static uint32_t *spf          = NULL;   /* smallest prime factor   */
    static uint32_t *prime_index   = NULL;   /* maps prime to its index */
    static size_t  terms_cap      = 0;
    static seq_t  *terms          = NULL;   /* cached sequence values  */
    static size_t  terms_len      = 0;
    static idx_t   current_m      = 2;       /* next candidate integer  */
    static bool    initialized    = false;

    if (!initialized) {
        /* build spf array */
        spf = (uint32_t *)malloc((size_t)(SPF_LIMIT + 1) * sizeof(uint32_t));
        for (idx_t i = 0; i <= SPF_LIMIT; ++i) spf[(size_t)i] = (uint32_t)i;
        for (idx_t i = 2; i * i <= SPF_LIMIT; ++i) {
            if (spf[(size_t)i] == (uint32_t)i) {
                for (idx_t j = i * i; j <= SPF_LIMIT; j += i)
                    if (spf[(size_t)j] == (uint32_t)j) spf[(size_t)j] = (uint32_t)i;
            }
        }

        /* build prime index table */
        prime_index = (uint32_t *)calloc((size_t)(SPF_LIMIT + 1), sizeof(uint32_t));
        size_t idx = 1;
        for (idx_t i = 2; i <= SPF_LIMIT; ++i)
            if (spf[(size_t)i] == (uint32_t)i) prime_index[(size_t)i] = (uint32_t)idx++;

        /* initial cache for sequence values */
        terms_cap = 256;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));

        initialized = true;
    }

    /* ------------------------------------------------------------ */
    /* generate terms until we reach the requested index          */
    /* ------------------------------------------------------------ */
    while (terms_len < (size_t)n) {
        if (current_m > SPF_LIMIT) {
            /* if we exceed the sieve limit, enlarge the sieve */
            /* (unlikely for n <= 10000) */
            enum { NEW_LIMIT = SPF_LIMIT * 2 };
            uint32_t *new_spf = (uint32_t *)malloc((size_t)(NEW_LIMIT + 1) * sizeof(uint32_t));
            for (idx_t i = 0; i <= NEW_LIMIT; ++i) new_spf[(size_t)i] = (uint32_t)i;
            for (idx_t i = 2; i * i <= NEW_LIMIT; ++i) {
                if (new_spf[(size_t)i] == (uint32_t)i) {
                    for (idx_t j = i * i; j <= NEW_LIMIT; j += i)
                        if (new_spf[(size_t)j] == (uint32_t)j) new_spf[(size_t)j] = (uint32_t)i;
                }
            }
            free(spf);
            spf = new_spf;

            uint32_t *new_prime_index = (uint32_t *)calloc((size_t)(NEW_LIMIT + 1), sizeof(uint32_t));
            size_t new_idx = 1;
            for (idx_t i = 2; i <= NEW_LIMIT; ++i)
                if (spf[(size_t)i] == (uint32_t)i) new_prime_index[(size_t)i] = (uint32_t)new_idx++;
            free(prime_index);
            prime_index = new_prime_index;
        }

        /* factor current_m using the spf array */
        idx_t tmp = current_m;
        uint32_t indices[64];
        size_t ind_len = 0;
        while (tmp > 1) {
            uint32_t p = spf[(size_t)tmp];
            indices[ind_len++] = prime_index[(size_t)p];
            tmp /= p;
        }

        /* sort asc so we can traverse from the end */
        qsort(indices, ind_len, sizeof(uint32_t), cmp_uint32_asc);

        /* compute reverse‑alternating product using 128‑bit intermediates */
        unsigned __int128 num = 1, den = 1;
        int sign = 1;                      /* start with +1 on the last element */
        for (int i = (int)ind_len - 1; i >= 0; --i, sign = -sign) {
            uint32_t val = indices[i];
            if (sign > 0) num *= (unsigned __int128)val;
            else          den *= (unsigned __int128)val;
        }

        /* include if product is not an integer */
        if (den != 0 && (num % den != (unsigned __int128)0)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)current_m;
        }

        ++current_m;
    }

    return terms[n - 1];
}
