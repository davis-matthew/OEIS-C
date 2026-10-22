/*
 * A332644  Largest of the least integers of prime signatures over all
 *          partitions of n into distinct parts.
 * https://oeis.org/A332644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 30, u128 n <= 51,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A332644:
 *   a(n) = the largest, over all partitions of n into distinct parts,
 *   of the least integer having the prime signature determined by that partition.
 *   The exponent pattern is given by:
 *     Let m be the largest integer with m(m+1)/2 ≤ n,
 *     r = n – m(m+1)/2.
 *     Exponents e_i (i = 1..m) are:
 *       e_i = (m – i + 1) + (i ≤ r ? 1 : 0).
 *     Then a(n) = Π_{i=1..m} p_i^{e_i}   (p_i = i‑th prime).
 *
 * All arithmetic is performed in the type seq_t, using the helpers
 *   big_pow (pow of a seq_t by a seq_t) and seq_mul (multiplication of seq_t).
 *
 * The first primes (up to the 2000th number) are precomputed once.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stddef.h>

/* helper functions provided by the environment */
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

static bool     primes_ready = false;
static size_t   prime_count  = 0;
static uint64_t primes[2000];   /* room for many primes, more than enough for n ≤ 10000 */

static void init_primes(void)
{
    const idx_t limit = 2000;
    bool sieve[limit + 1];
    size_t i, j;

    for (i = 0; i <= limit; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (j = i * i; j <= limit; j += i)
                sieve[j] = false;

    prime_count = 0;
    for (i = 2; i <= limit; ++i)
        if (sieve[i])
            primes[prime_count++] = (uint64_t)i;

    primes_ready = true;
}

seq_t a(idx_t n)
{
    /* Find m = floor( (sqrt(8n+1)-1)/2 )  by integer search */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    idx_t r = n - m * (m + 1) / 2;   /* leftover to distribute */

    if (!primes_ready)
        init_primes();

    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < m; ++i) {
        seq_t prime = (seq_t)primes[i];
        idx_t  exp   = (m - i) + (i < r ? 1 : 0); /* e_i = (m-i+1) + (i ≤ r ? 1 : 0) */
        seq_t power = big_pow(prime, (seq_t)exp);
        result = seq_mul(result, power);
    }

    return result;
}
