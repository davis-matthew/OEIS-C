/*
 * A358548  a(n) = A003627(n+1) - A003627(n).
 * https://oeis.org/A358548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the difference between consecutive primes of the form 3*k-1
   (i.e. the OEIS sequence A358548).  The implementation keeps an
   incremental sieve and fills `diffs` as more values are required. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static seq_t *diffs      = NULL;   /* array of differences */
static size_t diff_cap   = 0;       /* allocated capacity for `diffs` */
static idx_t  diff_cnt   = 0;       /* number of differences stored */

/* Highest integer that has been sieved so far. */
static idx_t sieve_limit = 0;
/* Last prime of the form 3*k-1 that has been processed. */
static seq_t last_prime;
static int   had_last   = 0;        /* flag: a last_prime value has been set */

/* Ensure that at least `need` differences are computed. */
static void ensure_available(idx_t need)
{
    if (diff_cnt >= need)
        return;

    while (diff_cnt < need) {
        /* Expand the sieve to a larger limit. */
        idx_t new_limit = (sieve_limit == 0) ? 20 : sieve_limit << 1;
        uint64_t *tmp   = NULL;
        size_t    tmp_cnt = 0;

        build_primes_upto(new_limit, &tmp, &tmp_cnt);

        for (size_t i = 0; i < tmp_cnt; ++i) {
            uint64_t p = tmp[i];
            if (p <= sieve_limit)
                continue;                 /* already processed */
            if ((p % 3u) != 2u)
                continue;                 /* not of the form 3*k-1 */

            seq_t sp = (seq_t)p;
            if (!had_last) {              /* first such prime */
                last_prime = sp;
                had_last = 1;
            } else {
                /* Grow the array if necessary. */
                if ((size_t)diff_cnt == diff_cap) {
                    size_t new_cap = (diff_cap == 0) ? 8 : diff_cap << 1;
                    seq_t *tmp_arr = (seq_t *)realloc(diffs,
                                                      new_cap * sizeof(seq_t));
                    if (!tmp_arr)
                        exit(EXIT_FAILURE);
                    diffs = tmp_arr;
                    diff_cap = new_cap;
                }
                diffs[(size_t)diff_cnt++] = sp - last_prime;
                last_prime = sp;
            }
        }

        free(tmp);
        sieve_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* index 0 is outside the offset (offset = 1) */
    ensure_available(n);      /* guarantee diff_cnt ≥ n */
    return diffs[(size_t)(n - 1)];
}
