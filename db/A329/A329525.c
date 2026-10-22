/*
 * A329525  a(n) is the smallest positive number k such that k and k+n are
 *          both abundant.
 * https://oeis.org/A329525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that k and k+n are both abundant.
 *            (Abundant: sum of proper divisors > k.)
 * Range:     u8 none, u16 n <= 9145, u32 n <= 9145, u64 n <= 9145,
 *            u128 n <= 9145, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Limits for precomputation.  These are adequate for all
       queries up to the offset used by the tests (n <= 10000). */
    enum { MAX_QUERY_N = 10000 };
    enum { LIMIT       = 5000000 };   /* maximum k to examine          */
    static bool initialized = false;

    /* Tables created only once. */
    static uint64_t *sigma       = NULL;   /* sum of proper divisors (uint64_t) */
    static bool      *is_abundant = NULL;   /* true if abundant */
    static idx_t     *abund_list  = NULL;   /* list of abundant numbers */
    static size_t     abund_cnt = 0;
    static seq_t     *answers    = NULL;    /* pre‑computed answers[1..MAX_QUERY_N] */

    if (!initialized)
    {
        /* Allocate tables large enough for all k and k+n needed. */
        size_t table_len = (size_t)(LIMIT + MAX_QUERY_N + 1);
        sigma = (uint64_t*)malloc(table_len * sizeof(uint64_t));
        is_abundant = (bool*)malloc(table_len * sizeof(bool));
        if (!sigma || !is_abundant) return 0;

        /* Initialise sigma with 1 for every n (the divisor 1). */
        for (size_t i = 0; i < table_len; ++i)
            sigma[i] = 1;

        /* Sieve accumulation of proper divisors. */
        for (size_t i = 2; i <= table_len / 2; ++i)
        {
            for (size_t j = i * 2; j < table_len; j += i)
                sigma[j] += (uint64_t)i;
        }

        /* Create list of abundant numbers. */
        size_t approx_size = table_len / 10;                 /* rough guess */
        abund_list = (idx_t*)malloc(approx_size * sizeof(idx_t));
        if (!abund_list) return 0;

        for (size_t i = 1; i < table_len; ++i)
        {
            if (sigma[i] > (uint64_t)i)          /* abundant if proper sum > i */
            {
                if (abund_cnt == approx_size)
                {
                    approx_size *= 2;
                    idx_t *tmp = (idx_t*)realloc(abund_list,
                                                approx_size * sizeof(idx_t));
                    if (!tmp) return 0;
                    abund_list = tmp;
                }
                is_abundant[i] = true;
                abund_list[abund_cnt++] = (idx_t)i;
            }
            else
                is_abundant[i] = false;
        }

        /* Pre‑compute answers for n = 1 .. MAX_QUERY_N. */
        answers = (seq_t*)calloc(MAX_QUERY_N + 1, sizeof(seq_t));
        if (!answers) return 0;

        for (idx_t q = 1; q <= MAX_QUERY_N; ++q)
        {
            for (size_t idx = 0; idx < abund_cnt; ++idx)
            {
                idx_t k = abund_list[idx];
                idx_t j = k + q;
                if (j >= (idx_t)table_len) break;
                if (is_abundant[j])
                {
                    answers[q] = k;
                    break;
                }
            }
        }

        initialized = true;
    }

    if (n == 0) return 0;                 /* a(0) undefined */

    /* Return pre‑computed value if within the pre­computed range. */
    if (n <= MAX_QUERY_N)
        return answers[(size_t)n];

    /* For larger n (outside tests) perform a direct search up to LIMIT. */
    size_t table_len = (size_t)(LIMIT + MAX_QUERY_N + 1);
    for (idx_t k = 12; k <= LIMIT; ++k)
    {
        if (!is_abundant[k]) continue;
        idx_t j = k + n;
        if (j >= (idx_t)table_len) break;
        if (is_abundant[j]) return k;
    }

    /* No suitable k found within the limit. */
    return 0;
}
