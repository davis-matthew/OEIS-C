/*
 * A252895  Numbers with an odd number of square divisors.
 * https://oeis.org/A252895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A252895: Numbers with an odd number of square divisors.
   A term n is in the sequence iff for every prime p dividing n,
   the exponent of p satisfies e ≡ 0 or 1 (mod 4).  The algorithm
   tests each integer in ascending order, using a dynamic SPF
   (smallest prime factor) sieve, until it has found the desired
   nth term. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- static tables for the SPF sieve --------------------------------- */
static uint32_t *spf   = NULL;   /* spf[i] = smallest prime factor of i      */
static size_t   spf_cap = 0;     /* highest index currently stored           */
static uint32_t *primes = NULL;  /* list of primes found                     */
static size_t   primes_cnt = 0;  /* number of primes                         */
static size_t   primes_cap = 0;  /* capacity of primes array                 */

/* Extend the SPF sieve to include all integers up to `new_limit`.
   The table is grown dynamically; all previous entries are retained. */
static void extend_spf(uint64_t new_limit)
{
    if (new_limit <= spf_cap)
        return;

    /* Resize the SPF array. */
    uint64_t old_cap = spf_cap;
    spf_cap = new_limit;
    spf = (uint32_t *)realloc(spf, (spf_cap + 1) * sizeof(uint32_t));
    if (!spf)
        abort();                     /* allocation failure */

    /* Initialise the new entries to 0. */
    for (uint64_t i = old_cap + 1; i <= new_limit; ++i)
        spf[i] = 0;

    /* Build the sieve (linear). */
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                primes = (uint32_t *)realloc(primes,
                                            primes_cap * sizeof(uint32_t));
                if (!primes)
                    abort();
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > new_limit)
                break;
            spf[(uint64_t)i * p] = p;
        }
    }
}

/* Return the nth term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, harmless */

    uint64_t target  = (uint64_t)n;
    uint64_t count   = 0;
    uint64_t cand    = 1;

    /* Start with a modest sieve size. */
    extend_spf(1024);

    for (;;) {
        if (cand > spf_cap)
            extend_spf(cand * 2);   /* grow the sieve if needed */

        /* Test whether `cand` satisfies the 0/1‑mod‑4 exponent condition. */
        uint64_t x  = cand;
        bool ok = true;
        while (x > 1) {
            uint32_t p  = spf[x];
            int exp = 0;
            do {
                x /= p;
                exp++;
            } while (x % p == 0);
            int r = exp % 4;
            if (r != 0 && r != 1) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ++count;
            if (count == target)
                return (seq_t)cand;
        }
        ++cand;
    }
}
