/*
 * A331544  Primes that begin a run of consecutive primes whose first
 *          differences are strictly increasing.
 * https://oeis.org/A331544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate all primes up to a growing limit, scan them to find
   maximal runs where consecutive prime gaps are strictly increasing, and
   return the k‑th run’s first prime.  The routine is memoised; it expands
   the prime list only until the requested index is covered. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;   /* array of primes */
static size_t primes_cnt = 0;     /* number of primes in the array */

static seq_t *runs = NULL;        /* array of first primes of runs */
static size_t runs_cnt = 0;       /* number of runs found */
static size_t runs_cap = 0;       /* capacity of the runs array */

static idx_t sieve_limit = 5000000;   /* initial prime bound */

/* Compute the first element of the k‑th run (k>=1). */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (primes == NULL) {
        /* keep generating larger lists until we have at least n runs */
        while (runs_cnt < n) {
            /* build prime list up to current limit */
            build_primes_upto(sieve_limit, &primes, &primes_cnt);

            /* scan primes to produce runs not yet stored */
            /* if we already have all runs from a previous pass, use that data */
            size_t start = 0;
            if (runs_cnt > 0) {
                /* locate the first prime we have not yet processed */
                /* this is the prime after the last run found */
                /* because runs are produced in order, the next start is
                   the prime after the last run element; we must find it */
                /* Re‑scan from the beginning each time for simplicity.
                   The overhead is negligible for the expected size. */
                start = 0;
            }

            /* produce runs from scratch each time to keep logic simple */
            size_t rcnt = 0;
            for (size_t i = 0; i + 1 < primes_cnt; /* no increment */) {
                /* current prime starts a run */
                if (rcnt == runs_cap) {
                    runs_cap = runs_cap ? runs_cap * 2 : 64;
                    runs = (seq_t *)realloc(runs,
                                            runs_cap * sizeof(seq_t));
                }
                runs[rcnt++] = (seq_t)primes[i];

                seq_t diff_prev = (seq_t)(primes[i + 1] - primes[i]);
                size_t j = i + 2;
                while (j < primes_cnt &&
                       (seq_t)(primes[j] - primes[j - 1]) > diff_prev) {
                    diff_prev = (seq_t)(primes[j] - primes[j - 1]);
                    ++j;
                }
                /* next run starts at prime where condition failed */
                i = j;
            }
            runs_cnt = rcnt;

            if (runs_cnt < n) { /* need more runs, increase limit */
                sieve_limit *= 2;
                free(primes);
                free(runs);
                primes = NULL;
                runs = NULL;
                runs_cnt = 0;
                runs_cap = 0;
                continue;
            }
            break;
        }
    }

    /* n is at least 1 and we have enough runs */
    return runs[(size_t)n - 1];
}
