/*
 * A350076  Numbers k for which the maximal digit in their primorial base
 *          expansion (A328114) is greater than or equal to the maximal
 *          exponent in the prime factorization of k (A051903).
 * https://oeis.org/A350076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer k (a(1)=1) for which
   max_primorial_digit(k) ≥ max_prime_exponent(k)
   where max_primorial_digit is the maximal digit in the primorial
   base expansion (OEIS A328114) and
   max_prime_exponent is the largest exponent in k's prime factorisation
   (OEIS A051903).  The function computes the sequence by
   incremental sieving of candidates. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Prime sieve storage */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Initialise a list of primes sufficient for factorisation up to
   ~200 000; this covers all candidates needed for n ≤ 10 000
   (the 10 000th term is well below 2 × 10⁵). */
static void init_primes(void)
{
    if (primes_cnt > 0) return;                  /* already done */

    const size_t limit = 200000;                 /* safety bound */
    primes_cap = 20000;
    primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    is_comp[j] = true;
                }
            }
        }
    }
    free(is_comp);
}

/* Return the largest exponent of any prime factor of n.
   For n ≤ 1 the result is 0. */
static uint64_t max_prime_exponent(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t max_ex = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            uint64_t e = 0;
            while (n % primes[i] == 0) {
                n /= primes[i];
                ++e;
            }
            if (e > max_ex) max_ex = e;
        }
    }
    if (n > 1 && max_ex < 1) max_ex = 1;   /* remaining prime factor */
    return max_ex;
}

/* Return the maximal digit in the primorial base representation of k.
   The representation is obtained by dividing k successively by the
   primes 2,3,5,7,... (the primorial base). */
static uint64_t max_primorial_digit(uint64_t k)
{
    uint64_t r = k;
    uint64_t mx = 0;
    size_t idx = 0;
    while (r > 0) {
        /* Ensure we have enough primes for the current index. */
        if (idx >= primes_cnt) {
            /* Generate the next prime. */
            uint64_t cand = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
            while (true) {
                bool is_p = true;
                for (size_t j = 0; j < primes_cnt && primes[j] * primes[j] <= cand; ++j) {
                    if (cand % primes[j] == 0) { is_p = false; break; }
                }
                if (is_p) break;
                ++cand;
            }
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = cand;
        }

        uint64_t p = primes[idx];
        uint64_t d = r % p;
        if (d > mx) mx = d;
        r /= p;
        ++idx;
    }
    return mx;
}

/* Cache of sequence values.  Storing values as seq_t allows
   arbitrary‑precision builds. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Next candidate integer to test for inclusion in the sequence. */
static uint64_t next_candidate = 1;

/* Grow the cache buffer to accommodate 'needed' entries. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of bounds, unused */
    init_primes();

    /* Generate candidates until we have at least n terms stored. */
    while (cache_len < (size_t)n) {
        uint64_t k = next_candidate;
        uint64_t d = max_primorial_digit(k);
        uint64_t e = max_prime_exponent(k);
        if (d >= e) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
