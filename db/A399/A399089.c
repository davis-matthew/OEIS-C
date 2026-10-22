/*
 * A399089  The maximum exponent in the prime factorization of the number
 *          preceding the n-th squarefree number.
 * https://oeis.org/A399089
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent in the prime factorization of the number
 *            immediately preceding the n‑th squarefree number. This
 *            implementation enumerates squarefree numbers up to the required
 *            index and factors the preceding number using a simple linear
 *            sieve for smallest prime factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count the maximum exponent for 1 ≤ x ≤ limit
   using the supplied smallest prime factor array. */
static idx_t max_exponent(uint64_t x, uint32_t *spf)
{
    idx_t max_exp = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        idx_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (cnt > max_exp)
            max_exp = cnt;
    }
    return max_exp;
}

seq_t a(idx_t n)
{
    /* undefined for n < 2 – return 0 */
    if (n < 2)
        return (seq_t)0;

    /* Estimate an upper bound for the nth squarefree number.
       Rough density of squarefree numbers ≈ 6/π² ≈ 0.6079, so we
       over‑estimate by a factor of 4 to be safe for all n in the
       intended range (up to about 10 000). */
    idx_t limit = n * 4 + 100;
    size_t sz = (size_t)(limit + 1);

    /* Allocate arrays.  C++ requires an explicit cast. */
    uint32_t *spf = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!spf) return (seq_t)0;        /* allocation failure */

    bool *sq_flag = (bool *)calloc(sz, sizeof(bool));
    if (!sq_flag) { free(spf); return (seq_t)0; }

    /* Linear sieve for smallest prime factor. */
    size_t primes_cap = 32;
    size_t primes_cnt = 0;
    uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) { free(spf); free(sq_flag); return (seq_t)0; }

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(spf); free(sq_flag); free(primes); return (seq_t)0; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t mult = (uint64_t)i * p;
            if (p > spf[i] || mult > (uint64_t)limit)
                break;
            spf[(size_t)mult] = p;
        }
    }

    /* Mark numbers divisible by a square of a prime. */
    for (size_t i = 1; i < sz; ++i) sq_flag[i] = true;
    sq_flag[0] = false;

    for (size_t j = 0; j < primes_cnt; ++j) {
        uint64_t p = primes[j];
        uint64_t p2 = p * p;
        if (p2 > (uint64_t)limit) break;
        for (uint64_t k = p2; k <= (uint64_t)limit; k += p2)
            sq_flag[(size_t)k] = false;
    }

    /* Find the n‑th squarefree number. */
    idx_t count = 0;
    uint64_t s_n = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (sq_flag[(size_t)i]) {
            ++count;
            if (count == n) {
                s_n = (uint64_t)i;
                break;
            }
        }
    }

    /* In the unlikely event we did not find it, increase limit and retry. */
    while (s_n == 0) {     /* safety fallback – should never happen */
        limit = limit * 2;
        sz = (size_t)(limit + 1);
        free(spf);
        free(sq_flag);
        free(primes);
        spf = (uint32_t *)calloc(sz, sizeof(uint32_t));
        if (!spf) return (seq_t)0;
        sq_flag = (bool *)calloc(sz, sizeof(bool));
        if (!sq_flag) { free(spf); return (seq_t)0; }
        primes_cap = 32;
        primes_cnt = 0;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) { free(spf); free(sq_flag); return (seq_t)0; }

        /* redo the sieve and marking as above */
        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!tmp) { free(spf); free(sq_flag); free(primes); return (seq_t)0; }
                    primes = tmp;
                }
                primes[primes_cnt++] = (uint32_t)i;
            }
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint32_t p = primes[j];
                uint64_t mult = (uint64_t)i * p;
                if (p > spf[i] || mult > (uint64_t)limit)
                    break;
                spf[(size_t)mult] = p;
            }
        }
        for (size_t i = 1; i < sz; ++i) sq_flag[i] = true;
        sq_flag[0] = false;
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            uint64_t p2 = p * p;
            if (p2 > (uint64_t)limit) break;
            for (uint64_t k = p2; k <= (uint64_t)limit; k += p2)
                sq_flag[(size_t)k] = false;
        }
        count = 0;
        s_n = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (sq_flag[(size_t)i]) {
                ++count;
                if (count == n) {
                    s_n = (uint64_t)i;
                    break;
                }
            }
        }
    }

    /* Compute maximum exponent of the preceding number. */
    uint64_t prev = s_n - 1;
    idx_t exp = 0;
    if (prev > 1)
        exp = max_exponent(prev, spf);

    /* Clean up */
    free(spf);
    free(sq_flag);
    free(primes);

    return (seq_t)exp;
}
