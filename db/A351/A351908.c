/*
 * A351908  Number of prime quadruples p < q < r < s in arithmetic progression
 *          with all members less than or equal to prime(n).
 * https://oeis.org/A351908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of prime quadruples p < q < r < s that
   form a 4‑term arithmetic progression with all members not exceeding the
   n‑th prime, i.e. prime(n).  The definition of the sequence A351908 is
   followed directly. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility prototypes from the build system.  The headers are named after
   the function.  The implementation of these utilities is provided by the
   host environment. */
#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k); */
#include "../utils/build_primes_upto.h"  /* void build_primes_upto(idx_t limit,
                                       uint64_t **primes_out, size_t *cnt_out); */

/* Return the a(n) value for the OEIS sequence A351908.  The function is
   safe for any n >= 1; for n < 1 the behavior is undefined (the offset is
   1).  It uses only integer arithmetic and memory allocations and
   therefore works for any of the provided seq_t types. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for the offset, but safe to return 0 */

    /* ----- 1.  Determine the upper bound prime(n).  ----- */
    uint32_t limit = get_prime(n); /* limit <= prime(10000) = 104729 for n<=10000 */

    /* ----- 2.  Generate all primes up to 'limit'.  ----- */
    uint64_t *all_primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &all_primes, &prime_cnt);

    /* ----- 3.  Build a boolean primality table up to 'limit'.  ----- */
    /* We use a byte per number; index 0..limit.  */
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    for (size_t i = 0; i < prime_cnt; ++i)
        is_prime[(size_t)all_primes[i]] = true;

    /* ----- 4.  Count all 4‑term arithmetic progressions.  ----- */
    seq_t count = 0;
    for (size_t i = 0; i + 3 < prime_cnt; ++i) {
        uint32_t p = (uint32_t)all_primes[i];
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint32_t r = (uint32_t)all_primes[j];
            uint32_t diff = r - p;           /* must be even for d integer */
            if (diff & 1U)
                continue;
            uint32_t d = diff >> 1;          /* d = (r-p)/2 */
            uint32_t q = p + d;
            uint32_t s = r + d;
            if (s > limit)
                break;                      /* larger r → larger s */
            if (is_prime[q] && is_prime[s])
                ++count;
        }
    }

    /* ----- 5.  Clean up and return.  ----- */
    free(all_primes);
    free(is_prime);
    return count;
}
