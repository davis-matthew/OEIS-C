/*
 * A272383  Numbers n such that Bernoulli number B_{n} has denominator 3318.
 * https://oeis.org/A272383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 47, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A272383 – Numbers n such that the denominator of the Bernoulli number B_n equals 3318.
 *
 *  Algorithm:
 *     – For n even > 0, the denominator of B_n is the product of all primes p
 *       with p‑1 | n.
 *     – The desired denominator 3318 = 2·3·7·79 requires that
 *         (i)  n is a multiple of 78   (lcm(1,2,6,78)),
 *         (ii)  no other prime p has p‑1 | n.
 *     – Therefore we mark all multiples of 78 as candidates and then
 *         clear those that are multiples of p‑1 for any prime p except
 *         p ∈ {2,3,7,79}.  The remaining numbers make the sequence.
 *     – The set is built once on first call and stored in a static cache.
 *     – The implementation keeps the cache large enough to answer all
 *         calls with n ≤ 10000 quickly (20000 terms pre‑generated).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache, built on first call */
    static seq_t *seq      = NULL;
    static size_t  seq_len = 0;
    static size_t  seq_cap = 0;

    /* upper limit to generate terms – far enough for n up to about 10 000 */
    const idx_t MMAX = 30000000ULL;      /* limit of sieved primes and candidates */
    const size_t TARGET = 20000;         /* number of terms to pre‑compute */

    if (seq == NULL) {
        /* ---------- 1. Sieve primes up to MMAX+1 ----------
           is_prime[p] == 1 iff p is prime (0 or 1 set to 0). */
        const idx_t LIMIT = MMAX + 1;
        uint8_t *is_prime = (uint8_t *)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));
        if (!is_prime)
            return (seq_t)0;  /* unlikely, but defensive */
        memset(is_prime, 1, (size_t)(LIMIT + 1));
        is_prime[0] = 0;
        is_prime[1] = 0;
        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = 0;
            }
        }

        /* collect all primes into a dynamic array */
        size_t primes_cap = 2000000;
        uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes)
            return (seq_t)0;
        size_t primes_cnt = 0;
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (is_prime[i]) {
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!tmp) { free(is_prime); free(primes); return (seq_t)0; }
                    primes = tmp;
                }
                primes[primes_cnt++] = (uint32_t)i;
            }
        }

        /* ---------- 2. Build allowed array ----------
           allowed[x] == 1 iff x satisfies the denominator condition. */
        uint8_t *allowed = (uint8_t *)calloc((size_t)(MMAX + 1), sizeof(uint8_t));
        if (!allowed) { free(is_prime); free(primes); return (seq_t)0; }

        /* all multiples of 78 are initial candidates */
        for (idx_t m = 78; m <= MMAX; m += 78)
            allowed[m] = 1;

        /* forbid multiples of p‑1 for all primes p except 2,3,7,79 */
        for (size_t k = 0; k < primes_cnt; ++k) {
            uint32_t p = primes[k];
            if (p == 2 || p == 3 || p == 7 || p == 79)
                continue;                 /* allowed primes */
            idx_t d = (idx_t)p - 1;        /* forbidden divisor */
            for (idx_t m = d; m <= MMAX; m += d) {
                if (allowed[m])
                    allowed[m] = 0;
            }
        }

        /* ---------- 3. Collect sequence into seq ----------
           Store the first TARGET terms. */
        seq_cap = TARGET * 2;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) { free(is_prime); free(primes); free(allowed); return (seq_t)0; }

        for (idx_t m = 78; m <= MMAX; ++m) {
            if (allowed[m]) {
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!tmp) { free(is_prime); free(primes); free(allowed); free(seq); return (seq_t)0; }
                    seq = tmp;
                }
                seq[seq_len++] = (seq_t)m;
                if (seq_len >= TARGET)
                    break;
            }
        }

        /* free temporary arrays */
        free(is_prime);
        free(allowed);
        free(primes);
    }

    /* ---------- 4. Return requested term ----------
       n is 1‑based; if out of range return 0. */
    if (n == 0 || n > seq_len)
        return (seq_t)0;
    return seq[n - 1];
}
