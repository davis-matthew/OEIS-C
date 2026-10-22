/*
 * A342301  Number of Goldbach partitions of 2n into an odd-indexed prime and
 *          an even-indexed prime.
 * https://oeis.org/A342301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Goldbach partitions of 2n into one odd‑indexed
 *            prime and one even‑indexed prime (computed via a sieve)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   sieve_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sieve_primes_upto.h"

/* binary search in a sorted array of 64‑bit integers */
static bool binary_search(const uint64_t *arr, size_t n, uint64_t value, size_t *out_index)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < value)
            lo = mid + 1;
        else if (arr[mid] > value)
            hi = mid;
        else {
            if (out_index) *out_index = mid;
            return true;
        }
    }
    return false;
}

/* seq_t and idx_t are provided by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined only for n >= 1 */

    /* Cache of primes up to the largest 2*n needed so far. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    idx_t two_n = n << 1;   /* 2*n, safe for uint64_t */

    if (two_n > primes_limit) {
        free(primes);
        sieve_primes_upto(two_n, &primes, &primes_cnt);
        primes_limit = two_n;
    }

    seq_t count = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n) break;                /* all larger primes give q < p */

        uint64_t q = two_n - p;
        if (q < p) break;                /* redundant but safe */

        /* check that q is prime */
        size_t idx_q;
        if (!binary_search(primes, primes_cnt, q, &idx_q))
            continue;                    /* not prime, skip */

        /* 1‑based indices */
        size_t idx_p = i + 1;            /* primes array 0‑based */
        size_t idx_q1 = idx_q + 1;

        bool p_even = (idx_p & 1) == 0;
        bool q_even = (idx_q1 & 1) == 0;

        if (p_even ^ q_even)            /* exactly one even‑indexed prime */
            ++count;
    }

    return count;
}
