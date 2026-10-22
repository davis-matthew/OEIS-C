/*
 * A339022  a(n) is the floor of the average distance among first n primes.
 * https://oeis.org/A339022
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 2/(n*(n-1)) * sum_{j=2..n} sum_{i=1..j-1}
 *            (prime(j)-prime(i)) )
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* One‑indexed storage for primes, their prefix sums, and the cumulative sum S[n] */
static uint64_t *primes   = NULL;   /* primes[1] = 2, primes[2] = 3, ... */
static seq_t   *prefix   = NULL;   /* prefix[0] = 0, prefix[k] = sum_{i=1..k} prime(i) */
static seq_t   *sums     = NULL;   /* sums[0] = 0, sums[k] = sum_{j=2..k} sum_{i=1..j-1} (prime(j)-prime(i)) */
static size_t   prime_cnt = 0;     /* number of primes stored */
static size_t   array_cap = 0;     /* current array capacity (≥ needed) */

/* Ensure the arrays can hold at least 'needed' 1–indexed entries. */
static void ensure_capacity(size_t needed)
{
    if (needed <= array_cap) return;

    size_t newcap = array_cap ? array_cap * 2 : 16;
    while (newcap < needed) newcap *= 2;

    primes = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
    prefix = (seq_t *)realloc(prefix, newcap * sizeof(seq_t));
    sums   = (seq_t *)realloc(sums,   newcap * sizeof(seq_t));
    array_cap = newcap;

    /* Initialise the zero index after each reallocation. */
    primes[0] = 0;
    prefix[0] = 0;
    sums[0]   = 0;
}

/* Generate primes up to at least the nth prime, updating prefix sums
   and the cumulative S[n] value. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    /* We need arrays large enough for 1‑indexed access up to n. */
    ensure_capacity((size_t)n + 1);

    uint64_t candidate = prime_cnt ? primes[prime_cnt] + 1 : 2;
    while ((size_t)prime_cnt < (size_t)n) {
        bool is_prime = true;

        if (candidate != 2) {
            if ((candidate & 1) == 0) { /* even */
                ++candidate;
                continue;
            }
            for (size_t i = 1; i <= prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) { is_prime = false; break; }
            }
        }

        if (is_prime) {
            ++prime_cnt;
            primes[prime_cnt] = candidate;

            /* Update prefix sum of primes. */
            prefix[prime_cnt] = prefix[prime_cnt - 1] + (seq_t)candidate;

            /* Update the cumulative S[prime_cnt].  For j = prime_cnt:
                 term = (j-1)*prime(j) - prefix[j-1] */
            seq_t term = ((seq_t)(prime_cnt - 1)) * (seq_t)candidate
                       - prefix[prime_cnt - 1];
            sums[prime_cnt] = sums[prime_cnt - 1] + term;
        }

        ++candidate;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* values below the offset are undefined */

    ensure_primes(n);

    seq_t numerator   = (seq_t)2 * sums[n];
    seq_t denominator = (seq_t)n * (seq_t)(n - 1);
    return numerator / denominator;     /* integer division = floor */
}
