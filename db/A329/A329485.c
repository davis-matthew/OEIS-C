/*
 * A329485  Odd numbers k such that there are no consecutive prime numbers p,
 *          q such that 2*(p - k) = q + k.
 * https://oeis.org/A329485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th odd integer k for which there is * no pair of
 *            consecutive primes p < q such that 2*(p - k) = q + k. *
 *            Equivalently, k is excluded iff k = (2p - q)/3 for some
 *            consecutive primes p,q. * * The algorithm builds primes up to a
 *            bound that is guaranteed to * cover all k needed for the re...
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, defensive return */

    /* Initial upper bound for k: roughly 3n; the constant overtakes the
     * actual growth and gives plenty of room for small n. */
    size_t bound_k = (size_t)n * 3 + 200;

    for (;;) {
        /* Need primes up to roughly 3·bound_k to generate all k ≤ bound_k. */
        size_t prime_limit = 3 * bound_k + 1000;

        /* --------- Sieve of Eratosthenes ---------- */
        unsigned char *sieve = (unsigned char *)malloc(prime_limit + 1);
        if (!sieve) return 0;
        for (size_t i = 0; i <= prime_limit; ++i) sieve[i] = 1;
        sieve[0] = sieve[1] = 0;
        for (size_t i = 2; i * i <= prime_limit; ++i)
            if (sieve[i])
                for (size_t j = i * i; j <= prime_limit; j += i)
                    sieve[j] = 0;

        /* Count primes to allocate array. */
        size_t primes_cnt = 0;
        for (size_t i = 2; i <= prime_limit; ++i)
            if (sieve[i]) ++primes_cnt;

        uint64_t *primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
        if (!primes) { free(sieve); return 0; }
        size_t pc = 0;
        for (size_t i = 2; i <= prime_limit; ++i)
            if (sieve[i]) primes[pc++] = (uint64_t)i;

        free(sieve);                     /* sieve is no longer needed */

        /* ---------- Mark excluded k values ---------- */
        unsigned char *excluded = (unsigned char *)calloc(bound_k + 1, 1);
        if (!excluded) { free(primes); return 0; }

        for (size_t i = 0; i + 1 < primes_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t q = primes[i + 1];
            uint64_t diff = 2 * p - q;            /* diff = 3k */
            if (diff % 3 == 0) {
                uint64_t k = diff / 3;
                if (k >= 1 && (k & 1ull) && k <= (uint64_t)bound_k)
                    excluded[(size_t)k] = 1;
            }
        }

        /* ---------- Find the n‑th non‑excluded odd k ---------- */
        idx_t count = 0;
        seq_t answer = 0;
        for (seq_t k = 1; k <= (seq_t)bound_k; k += 2) {
            if (!excluded[(size_t)k]) {
                ++count;
                if (count == n) {
                    answer = k;
                    break;
                }
            }
        }

        free(primes);
        free(excluded);

        if (count >= n) return answer;   /* success */

        /* Not enough terms yet – increase bound and repeat. */
        bound_k *= 2;
    }
}
