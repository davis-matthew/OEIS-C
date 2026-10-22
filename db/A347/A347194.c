/*
 * A347194  Numbers such that the two adjacent integers are a prime and the
 *          square of another prime.
 * https://oeis.org/A347194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   append_candidate, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A347194: numbers k such that k-1 and k+1 are a prime and a
   square of a distinct prime.
   For each prime q, if q²-2 or q²+2 is prime then
       k = q²-1   (when q²-2 is prime)
       k = q²+1   (when q²+2 is prime) 
   The terms are produced in increasing order.
*/
#include "../utils/append_candidate.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next integer to test for primality as the candidate prime q */
    static idx_t next_q = 2;

    /* a(1) and larger are valid; n==0 never used but guard anyway */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* find the next prime q */
        for (;;) {
            if (is_prime_small((seq_t)next_q))
                break;
            ++next_q;
        }

        seq_t q_sq = (seq_t)next_q * (seq_t)next_q;

        /* q²-2 prime and distinct from q -> k = q²-1 */
        if (is_prime_small(q_sq - 2) && (seq_t)next_q != (q_sq - 2)) {
            append_candidate(&cache, &cache_len, &cache_cap, q_sq - 1);
        }

        /* q²+2 prime -> k = q²+1 */
        if (is_prime_small(q_sq + 2)) {
            append_candidate(&cache, &cache_len, &cache_cap, q_sq + 1);
        }

        ++next_q; /* advance to next integer for the next iteration */
    }

    /* return the nth term (1‑based) */
    return cache[n - 1];
}
