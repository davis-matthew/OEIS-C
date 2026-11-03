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
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* simple prime sieve up to limit (inclusive) */
static void sieve_primes_upto(idx_t limit, uint64_t **prime_arr, size_t *cnt)
{
    if (limit < 2) {
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    uint8_t *is_comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_comp) {
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t est = (size_t)(limit / 2 + 10);
    uint64_t *pr = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!pr) {
        free(is_comp);
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (c < est)
                pr[c++] = (uint64_t)i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = (idx_t)i * (idx_t)i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    *prime_arr = pr;
    *cnt = c;
}

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
