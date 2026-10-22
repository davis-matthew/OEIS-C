/*
 * A376267  Run-lengths of first differences (A078147) of nonsquarefree
 *          numbers (A013929).
 * https://oeis.org/A376267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the length of the nth run of equal differences between
 *            consecutive nonsquarefree numbers. The list of nonsquarefree
 *            numbers is: 4,8,9,12,16,18,20,… Their first differences are:
 *            4,1,3,4,2,2,4,1,2,1,4,4,4,4,1,3,1,1,2,2,2,4,3,1,… This function
 *            returns the run‑lengths of that difference list:...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

/* static state --------------------------------------------------------- */

/* true if the number is squarefree, false otherwise */
static bool        *squarefree  = NULL;
/* current limit of the sieve (inclusive) */
static size_t       sieve_limit = 0;

static seq_t       *runlen      = NULL;
static size_t       runlen_cap  = 0;
static size_t       runlen_len  = 0;

static idx_t        next_check   = 2;    /* next integer to test */
static idx_t        prev_nsf     = 0;    /* last nonsquarefree number seen */
static bool         has_prev_nsf = 0;    /* true once we have seen one */
static idx_t        prev_diff    = 0;    /* difference of last two */
static idx_t        curr_run     = 0;    /* run length of current difference */

static bool         initialized  = 0;    /* 1 if the state has been set up */

static void init_sieve(size_t limit)
{
    if (squarefree != NULL) {
        free(squarefree);
    }
    squarefree = (bool*)malloc((limit + 1) * sizeof(bool));
    if (squarefree == NULL) exit(EXIT_FAILURE);

    /* mark all as squarefree first */
    memset(squarefree, 1, limit + 1);

    /* eliminate numbers that have a squared prime factor */
    for (idx_t i = 2; i * i <= limit; ++i) {
        idx_t sq = i * i;
        for (idx_t j = sq; j <= limit; j += sq) {
            squarefree[j] = 0;
        }
    }
    sieve_limit = limit;
}

static void extend_sieve(void)
{
    size_t new_limit = sieve_limit ? sieve_limit * 2 : 1000000;
    init_sieve(new_limit);
}

/* ensure runlen contains at least 'n' values */
static void ensure_runlen(idx_t n)
{
    if (runlen_len >= (size_t)n) return;

    if (!initialized) {
        init_sieve(1000000);      /* start with 1 000 000 */
        runlen_cap   = 0;
        runlen       = NULL;
        next_check   = 2;
        has_prev_nsf = 0;
        curr_run     = 0;
        initialized  = 1;
    }

    while (runlen_len < (size_t)n) {
        if (next_check > sieve_limit) {
            extend_sieve();
        }

        for ( ; next_check <= sieve_limit && runlen_len < (size_t)n; ++next_check ) {
            if (!squarefree[next_check]) {     /* nonsquarefree number found */
                if (!has_prev_nsf) {
                    prev_nsf     = next_check;
                    has_prev_nsf = 1;
                    curr_run = 0;            /* no run yet */
                } else {
                    idx_t diff = next_check - prev_nsf;
                    if (curr_run == 0) {            /* first difference seen */
                        curr_run  = 1;
                        prev_diff = diff;
                    } else if (diff == prev_diff) {
                        ++curr_run;
                    } else {
                        /* run finished – store it */
                        if (runlen_len == runlen_cap) {
                            runlen_cap = runlen_cap ? runlen_cap * 2 : 256;
                            runlen     = (seq_t*)realloc(runlen, runlen_cap * sizeof(seq_t));
                            if (!runlen) exit(EXIT_FAILURE);
                        }
                        runlen[runlen_len++] = (seq_t)curr_run;
                        prev_diff = diff;
                        curr_run  = 1;
                    }
                    prev_nsf = next_check;
                }
            }
        }

        /* if we ran out of pre‑computed numbers we still have an unfinished run */
        if (curr_run > 0 && runlen_len < (size_t)n) {
            if (runlen_len == runlen_cap) {
                runlen_cap = runlen_cap ? runlen_cap * 2 : 256;
                runlen     = (seq_t*)realloc(runlen, runlen_cap * sizeof(seq_t));
                if (!runlen) exit(EXIT_FAILURE);
            }
            runlen[runlen_len++] = (seq_t)curr_run;
            curr_run = 0;
        }
    }
}

/* public API ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1‑based indexing */
    ensure_runlen(n);
    return runlen[n - 1];
}
