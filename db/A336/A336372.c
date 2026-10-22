/*
 * A336372  Primes prime(k) such that gcd(k, prime(k) + prime(k-1)) = 1.
 * https://oeis.org/A336372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = the nth prime p_k such that gcd(k, p_k + p_{k-1}) = 1,
   where k is the prime's position in the list of all primes.  The function
   builds the required primes on demand and memoises them so that each call
   can handle arbitrary order of n.  The implementation is O(n) with a
   simple trial‑division primality test, suitable for n up to about 10 000. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

#define MAX_PRIMES 100000   /* enough for all n ≤ 10 000 */

static seq_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;
static uint64_t next_candidate = 2;

static seq_t qual[MAX_PRIMES];
static size_t qual_cnt = 0;

static inline bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    /* quick test for even numbers */
    if (n > 2 && (n & 1) == 0) return false;

    /* integer sqrt (rounded down) */
    uint64_t r = (uint64_t)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] <= r; ++i) {
        if (n % (uint64_t)primes[i] == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour below 1 is unused */

    while (qual_cnt < n) {
        /* find next prime */
        uint64_t cand;
        do {
            cand = next_candidate++;
            /* skip even candidates after 2 */
            if (cand > 2 && (cand & 1) == 0) continue;
            if (is_prime_candidate(cand)) {
                /* store prime */
                if (primes_cnt >= MAX_PRIMES) return 0; /* safety guard */
                primes[primes_cnt++] = (seq_t)cand;

                /* check the condition for this prime (k = primes_cnt) */
                if (primes_cnt >= 2) {
                    seq_t sum = primes[primes_cnt - 1] + primes[primes_cnt - 2];
                    seq_t g   = gcd((seq_t)primes_cnt, sum);
                    if (g == (seq_t)1) {
                        if (qual_cnt >= MAX_PRIMES) return 0;
                        qual[qual_cnt++] = primes[primes_cnt - 1];
                    }
                }
                break;          /* prime found and processed */
            }
        } while (true);
    }

    return qual[n - 1];
}
