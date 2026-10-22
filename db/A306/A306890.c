/*
 * A306890  a(n) is the number of prime digits used in writing out all primes
 *          up to and including the n-th prime.
 * https://oeis.org/A306890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=1}^n count_prime_digits(p_k) where p_k is the k‑th
 *            prime. The sequence is computed by generating all primes up to
 *            the n‑th one and accumulating the number of prime digits
 *            (2,3,5,7) in each prime
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Count the prime digits in a non‑negative integer. */
static seq_t count_prime_digits(idx_t x)
{
    seq_t cnt = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        switch (d) {
            case 2: case 3: case 5: case 7: ++cnt; break;
            default:                 break;
        }
        x /= 10;
    }
    return cnt;
}

/* Static cache of primes and the cumulative prime‑digit sums. */
static idx_t *primes      = NULL;
static seq_t *cum         = NULL;
static size_t primes_cnt  = 0;
static size_t primes_cap  = 0;

/* Ensure the cache can hold at least `needed` entries. */
static void ensure_capacity(size_t needed)
{
    if (primes_cap >= needed)
        return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    cum    = (seq_t *)realloc(cum,    new_cap * sizeof(seq_t));
    primes_cap = new_cap;
}

/* Generate primes until we have at least `target` primes in the cache. */
static void generate_primes_upto(idx_t target)
{
    while (primes_cnt < target) {
        /* Find the next candidate prime. */
        idx_t cand = (primes_cnt == 0) ? 2ULL : primes[primes_cnt - 1] + 1ULL;
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                idx_t p = primes[i];
                if (p * p > cand)           /* p exceeds sqrt(cand) */
                    break;
                if (cand % p == 0ULL) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            ++cand;
        }

        /* Store the new prime and update the cumulative sum. */
        ensure_capacity(primes_cnt + 1);
        seq_t dcnt = count_prime_digits(cand);
        seq_t prev = (primes_cnt == 0) ? 0 : cum[primes_cnt - 1];
        cum[primes_cnt] = prev + dcnt;
        primes[primes_cnt] = cand;
        ++primes_cnt;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    generate_primes_upto((idx_t)n);
    return cum[n - 1];
}
