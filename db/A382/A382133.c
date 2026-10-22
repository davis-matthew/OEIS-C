/*
 * A382133  Products of 4 distinct primes that are the average of two
 *          consecutive primes.
 * https://oeis.org/A382133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   append_candidate, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382133:
   Products of 4 distinct primes that are the average of two consecutive primes.  */
/* 
   We generate primes up to a fixed bound using the supplied sieve helper, then
   scan consecutive prime pairs. For each pair whose average is an integer,
   we test whether that average is the product of exactly four distinct primes.
   Once a term is found, it is appended to a growing cache.  The function
   returns terms on demand, memoising previously computed values so that
   subsequent calls may use the cached sequence.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/append_candidate.h"

/* Prime sieve state – built once. */
static uint64_t *prime_list = NULL;
static size_t      prime_cnt  = 0;
static bool        primes_init = false;

/* Sequence cache state. */
static seq_t  *term_list   = NULL;
static size_t term_len     = 0;
static size_t term_cap     = 0;

/* Next prime pair index to examine. */
static size_t pair_start = 1;

/* Initialise primes once. */
static void init_primes(void)
{
    if (!primes_init) {
        /* 20 million is ample for about 10 000 terms. */
        build_primes_upto(20000000ULL, &prime_list, &prime_cnt);
        primes_init = true;
    }
}

/* Generate sequence terms up to the requested target index. */
static void generate_terms_to(size_t target)
{
    init_primes();

    size_t i;
    for (i = pair_start; i + 1 < prime_cnt && term_len < target; ++i) {
        uint64_t p1 = prime_list[i];
        uint64_t p2 = prime_list[i + 1];

        /* Sum must be even for the average to be an integer. */
        if ((p1 + p2) & 1ULL)
            continue;

        /* Average of the pair. */
        seq_t avg = (seq_t)(p1 + p2) >> 1;

        /* Factorise avg to count distinct primes. */
        seq_t   t    = avg;
        size_t  cnt  = 0;
        seq_t   prod = 1;
        bool    ok   = true;

        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_list[j];
            seq_t sp = (seq_t)p;

            if (sp * sp > t)
                break;

            if (t % sp == 0) {
                t /= sp;
                if (t % sp == 0) {   /* repeated prime factor */
                    ok = false;
                    break;
                }
                ++cnt;
                prod *= sp;
            }
        }

        if (!ok)
            continue;

        if (t > 1) {          /* remaining prime factor > sqrt(avg) */
            ++cnt;
            prod *= t;
        }

        if (cnt == 4 && prod == avg)
            append_candidate(&term_list, &term_len, &term_cap, (seq_t)avg);
    }

    /* When the loop exits, i equals the next pair index to be examined. */
    pair_start = i;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* behaviour below offset is undefined */

    size_t need = (size_t)n;

    if (term_len < need)
        generate_terms_to(need);

    return term_list[need - 1];
}
