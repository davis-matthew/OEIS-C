/*
 * A298904  a(n) is the number of distinct primes produced by starting with
 *          the n-th prime p and repeatedly looking at all the prime factors
 *          of 2p-1, and then performing the same process (double, subtract 1,
 *          find all prime factors) with those primes; a(n) = -1 if this
 *          produces infinitely many primes.
 * https://oeis.org/A298904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes obtained from the n‑th prime by
 *            iteratively taking the prime factors of 2p − 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   contains_uint64, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t)  – defined in test harness */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */
#include "../utils/contains_uint64.h"

seq_t a(idx_t n)
{
    /* Starting prime */
    uint64_t start_prime = (uint64_t)get_prime(n);
    if (start_prime == 0)   /* safety check */
        return (seq_t)0;

    /* Dynamic arrays for seen primes and BFS queue */
    size_t seen_cap = 64, seen_len = 0;
    uint64_t *seen = (uint64_t *)malloc(seen_cap * sizeof(uint64_t));
    if (!seen) return (seq_t)0;

    size_t q_cap = 64, q_head = 0, q_tail = 0;
    uint64_t *queue = (uint64_t *)malloc(q_cap * sizeof(uint64_t));
    if (!queue) {
        free(seen);
        return (seq_t)0;
    }

    /* Initialise with the starting prime */
    seen[seen_len++] = start_prime;
    queue[q_tail++] = start_prime;

    /* BFS */
    while (q_head < q_tail) {
        uint64_t cur = queue[q_head++];
        /* compute m = 2*cur - 1; guard against 128‑bit overflow */
        unsigned __int128 tmp = (unsigned __int128)cur * 2 - 1;
        if (tmp > UINT64_MAX)
            break;  /* unlikely for the ranges considered; abort early */

        uint64_t m = (uint64_t)tmp;

        /* factor m */
        uint64_t pr[64], ex[64];
        size_t cnt = 0;
        factor_u64(m, pr, ex, &cnt);

        /* process distinct prime factors */
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t r = pr[i];
            if (r == 0 || r == 1)
                continue;   /* skip non‑primes (should not happen) */

            if (!contains_uint64(seen, seen_len, r)) {
                /* add to seen list */
                if (seen_len == seen_cap) {
                    seen_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(seen, seen_cap * sizeof(uint64_t));
                    if (!tmp) { free(seen); free(queue); return (seq_t)0; }
                    seen = tmp;
                }
                seen[seen_len++] = r;

                /* enqueue for further processing */
                if (q_tail == q_cap) {
                    q_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(queue, q_cap * sizeof(uint64_t));
                    if (!tmp) { free(seen); free(queue); return (seq_t)0; }
                    queue = tmp;
                }
                queue[q_tail++] = r;
            }
        }
    }

    /* clean up and return result */
    free(seen);
    free(queue);
    return (seq_t)seen_len;
}
