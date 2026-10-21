/*
 * A254130  Numbers whose factorials are exclusionary: numbers n such that n
 *          and n! have no digits in common.
 * https://oeis.org/A254130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_frequencies, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254130: numbers n such that n and n! have no digits in common.
   The code searches for terms on demand and memoises them. */

#include <stdlib.h>
#include <stdbool.h>
/* helpers for multiplying by a small integer and obtaining digit frequencies */
#include "../utils/mul_small.h"
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* cache of previously found terms */
    static seq_t *terms    = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* current candidate and its factorial */
    static idx_t current_candidate = 0;  /* 0,1,2,... */
    static seq_t current_fact = 1;       /* factorial: 0! = 1 */

    /* compute terms up to requested index */
    while (terms_len < n)
    {
        unsigned int freq_c[10] = {0};
        unsigned int freq_f[10] = {0};

        /* digit sets of the candidate number and of its factorial */
        digit_frequencies((seq_t)current_candidate, freq_c);
        digit_frequencies(current_fact, freq_f);

        /* test for common digits */
        bool ok = true;
        for (int d = 0; d < 10; ++d)
        {
            if (freq_c[d] && freq_f[d])
            {
                ok = false;
                break;
            }
        }

        /* store term if it satisfies the property */
        if (ok)
        {
            if (terms_len + 1 > terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms     = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)current_candidate;
        }

        /* advance to next candidate and update factorial */
        ++current_candidate;
        current_fact = mul_small(current_fact, (unsigned)current_candidate);
    }

    return terms[n - 1];
}
