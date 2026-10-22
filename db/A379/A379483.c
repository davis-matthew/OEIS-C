/*
 * A379483  a(n) is the number of trailing 1-bits in the binary representation
 *          of sigma(A003961(n^2)), where A003961 is fully multiplicative with
 *          a(prime(i)) = prime(i+1), and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A379483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of trailing 1‑bits in binary of σ(A003961(n²))
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 1636, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   build_primes_upto, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_seq.h"


static uint64_t *g_primes = NULL;
static size_t g_prime_cnt = 0;
static bool g_primes_built = false;

/* Initialise the list of primes up to 20000 (enough for next primes of
   primes ≤ 10000).  The array is static to avoid rebuilding it. */
static void init_primes(void)
{
    if (g_primes_built) return;
    uint64_t *primes_out = NULL;
    size_t cnt_out = 0;
    build_primes_upto(20000, &primes_out, &cnt_out);
    g_primes = primes_out;
    g_prime_cnt = cnt_out;
    g_primes_built = true;
}

/* Return the next prime strictly greater than p.  The prime table is
   guaranteed to contain a prime larger than any p ≤ 10000. */
static uint64_t next_prime_of(uint64_t p)
{
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        if (g_primes[i] > p) return g_primes[i];
    }
    return 0;   /* should never happen for the problem constraints */
}

/* Compute a(n).  The routine works for any seq_t that supports the
   arithmetic and bitwise operators mentioned in the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* expression is undefined for n = 0 */
    init_primes();

    uint64_t m = (uint64_t)n;        /* copy of n for factorisation */
    seq_t sigma = 1;                 /* product of the σ‑terms for each prime factor */

    /* Factorise m; for each prime factor p with exponent e in m,
       compute the σ‑term contributed by nextprime(p)^(2e). */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            uint64_t q = next_prime_of(p);
            int exp = (int)(2 * e + 1);   /* exponent for the σ‑term */
            seq_t powq = pow_seq((seq_t)q, exp);
            seq_t numer = powq - 1;
            seq_t denom = (seq_t)q - 1;
            seq_t term = numer / denom;
            sigma *= term;
        }
    }

    /* If m > 1 now, it is a prime > sqrt(original n), exponent 1. */
    if (m > 1) {
        uint64_t p = m;
        uint64_t q = next_prime_of(p);
        int exp = 3;                      /* 2*1 + 1 */
        seq_t powq = pow_seq((seq_t)q, exp);
        seq_t numer = powq - 1;
        seq_t denom = (seq_t)q - 1;
        seq_t term = numer / denom;
        sigma *= term;
    }

    /* Count trailing 1‑bits in sigma. */
    seq_t count = 0;
    while ((sigma & 1) != 0) {
        sigma >>= 1;
        ++count;
    }
    return count;
}
