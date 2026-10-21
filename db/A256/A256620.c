/*
 * A256620  Numbers n such that n is both the average of some twin prime pair
 *          p, q (q = p+2) (i.e., n = p+1 = q-1) and is also the arithmetic
 *          mean of the four numbers consisting of the two primes before p and
 *          the two primes after q.
 * https://oeis.org/A256620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 9202, u64 n <= 9202,
 *            u128 n <= 9202, big any n
 * Helpers:   alloc_seq_array, build_primes_upto, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A256620:  numbers n that are the average of a twin prime pair p,q (q=p+2)
 *  and also the arithmetic mean of the two primes before p and the two primes after q.
 *  The function lazily constructs primes and the sequence as needed, doubling the sieve
 *  limit until the requested term is found.  It is written to be valid C11 and C++17
 *  and uses the provided utility functions for prime generation and sequence storage.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, &primes, &cnt)   */
#include "../utils/alloc_seq_array.h"    /* alloc_seq_array(count) -> seq_t*           */
#include "../utils/free_seq.h"           /* free_seq(ptr)                              */

seq_t a(idx_t n) {
    /* Static state */
    static seq_t *terms = NULL;
    static idx_t term_count = 0;
    static idx_t sieve_limit = 0;
    bool initialized = (terms != NULL);

    if (!initialized) {
        terms = NULL;
        term_count = 0;
        sieve_limit = 0;
    }

    if (n == 0) return (seq_t)0;       /* undefined, but safe */

    /* Ensure we have at least n terms */
    while (term_count < n) {
        /* Choose the next sieve limit: start at 1e6, then double each time */
        idx_t new_limit = (sieve_limit == 0 ? 1000000 : sieve_limit * 2);

        /* Generate primes up to new_limit */
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes_upto(new_limit, &primes, &prime_cnt);
        if (primes == NULL || prime_cnt < 4) {
            /* Something went wrong; abort */
            return (seq_t)0;
        }

        /* Recompute the sequence from scratch for this limit *
         * (we discard the previous terms – this is acceptable
         *  because dropping earlier terms and recomputing does not affect
         *  correctness, only performance). */
        free_seq(terms);
        terms = NULL;
        term_count = 0;

        /* Estimate capacity for terms */
        size_t est = prime_cnt / 10 + 10;
        seq_t *new_terms = (seq_t *)alloc_seq_array(est);
        size_t tcnt = 0;

        for (size_t i = 0; i + 3 < prime_cnt; ++i) {
            /* Check for a twin prime pair p (=primes[i]), q (=primes[i+1]) */
            if (primes[i + 1] == primes[i] + 2) {
                /* Need two primes before p: i-2, i-1 (so i>=2) */
                if (i < 2) continue;
                /* Need two primes after q: i+2, i+3 (already ensured by loop bound) */
                uint64_t sum  = primes[i - 2] + primes[i - 1] + primes[i + 2] + primes[i + 3];
                uint64_t avg  = primes[i] + 1ULL;        /* n = p+1 = q-1 */
                if (sum == avg * 4ULL) {
                    if (tcnt >= est) {
                        /* Grow the array */
                        est *= 2;
                        seq_t *tmp = (seq_t *)alloc_seq_array(est);
                        for (size_t j = 0; j < tcnt; ++j) tmp[j] = new_terms[j];
                        free_seq(new_terms);
                        new_terms = tmp;
                    }
                    new_terms[tcnt++] = (seq_t)avg;
                }
            }
        }

        free(primes);
        free_seq(terms);
        terms = new_terms;
        term_count = tcnt;
        sieve_limit = new_limit;

        /* If the new limit yielded no new terms, stop to avoid infinite loop */
        if (term_count == 0) break;
    }

    /* Return the n-th term (1‑based) if available */
    if (n <= term_count) {
        return terms[n - 1];
    }
    return (seq_t)0;   /* If we ran out of terms, return 0 (rare) */
}
