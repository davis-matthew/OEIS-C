/*
 * A360249  Numbers for which the prime indices have the same median as the
 *          distinct prime indices.
 * https://oeis.org/A360249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer for which the median of its
 *            multiset of prime indices equals the median of its distinct
 *            prime indices. The sequence starts
 *            1,2,3,4,5,6,7,8,9,10,11,13,14,15,16,17,19,21,22,23,25,26,27,29,...
 *            A brute‑force search is used, incrementally testing numbers
 *            until the nth...
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   build_primes_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_sieve.h"

/* ---- binary search for the index of a prime (1‑based) in the primes array ---- */
static size_t prime_index(const uint64_t *primes, size_t cnt, uint64_t p)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p)
            return mid;  /* zero based */
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return cnt;  /* not found – should not happen for valid calls */
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    /* prime limit – large enough for n up to ~10000 (the density of terms is
       sufficiently high that the 10000th term stays below 20 000 000).        */
    const uint64_t SIEVE_LIMIT = 20000000ULL;

    if (!initialized) {
        build_primes_sieve(SIEVE_LIMIT, &primes, &primes_cnt);
        initialized = true;
    }

    size_t found = 0;
    uint64_t candidate = 1;

    while (found < (size_t)n) {
        /* candidate == 1 is a special case */
        if (candidate == 1) {
            ++found;
            if (found == (size_t)n)
                return (seq_t)candidate;
            ++candidate;
            continue;
        }

        uint64_t m = candidate;
        /* store prime indices (1‑based) and their exponents */
        uint32_t idxs[64];
        uint32_t exps[64];
        size_t k = 0;

        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= m; ++i) {
            uint64_t p = primes[i];
            if (m % p == 0) {
                uint32_t e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                idxs[k] = (uint32_t)(i + 1);  /* 1‑based prime index */
                exps[k] = e;
                ++k;
            }
        }
        if (m > 1) {          /* remaining prime factor (must be <= SIEVE_LIMIT) */
            size_t pos = prime_index(primes, primes_cnt, m);
            if (pos == primes_cnt)    /* safety: never happens because m is prime */
                pos = primes_cnt - 1;
            idxs[k] = (uint32_t)(pos + 1);
            exps[k] = 1;
            ++k;
        }

        /* ----- multiset median ----------------------------------- */
        size_t total_len = 0;
        for (size_t i = 0; i < k; ++i)
            total_len += (size_t)exps[i];

        size_t multiset_num, multiset_den;
        if (total_len % 2 == 1) {             /* odd length */
            size_t target = total_len / 2;
            size_t sum = 0;
            for (size_t i = 0; i < k; ++i) {
                sum += (size_t)exps[i];
                if (sum > target) {
                    multiset_num = idxs[i];
                    multiset_den = 1;
                    break;
                }
            }
        } else {                               /* even length */
            size_t pos1 = total_len / 2 - 1;
            size_t pos2 = total_len / 2;
            size_t sum = 0;
            size_t val1 = 0, val2 = 0;
            for (size_t i = 0; i < k; ++i) {
                sum += (size_t)exps[i];
                if (val1 == 0 && sum > pos1)
                    val1 = idxs[i];
                if (val2 == 0 && sum > pos2)
                    val2 = idxs[i];
                if (val1 && val2)
                    break;
            }
            multiset_num = val1 + val2;
            multiset_den = 2;
        }

        /* ----- distinct indices median ----------------------------- */
        size_t distinct_num, distinct_den;
        if (k % 2 == 1) {
            distinct_num = idxs[k / 2];
            distinct_den = 1;
        } else {
            distinct_num = idxs[k / 2 - 1] + idxs[k / 2];
            distinct_den = 2;
        }

        /* compare fractions without overflow */
        if (multiset_num * distinct_den == distinct_num * multiset_den) {
            ++found;
            if (found == (size_t)n)
                return (seq_t)candidate;
        }

        ++candidate;
    }
    /* control never reaches here */
    return (seq_t)0;
}
