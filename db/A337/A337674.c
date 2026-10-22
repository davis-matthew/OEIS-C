/*
 * A337674  Numbers k whose prime divisors are all less than or equal to the
 *          number of divisors of k.
 * https://oeis.org/A337674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A337674.
   A number k is in the sequence if every prime divisor p of k satisfies
   p ≤ τ(k), where τ(k) is the number of divisors of k.  The function
   generates terms by brute‑force testing each integer until the desired
   index is reached. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Factorize a uint64_t into distinct primes and their exponents.
   The buffers `primes` and `exps` must be large enough to hold the
   result.  The count is written to *cnt. */
static void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t i = 0;
    uint64_t d = 2;
    *cnt = 0;

    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[i] = d;
            exps[i] = e;
            ++i;
        }
        d = (d == 2) ? 3 : d + 2;   /* skip even numbers after 2 */
    }
    if (n > 1) {
        primes[i] = n;
        exps[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache: stored sequence values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current allocated capacity */
    static size_t len   = 0;   /* number of cached terms */
    static uint64_t cand = 0;  /* next candidate to test */

    while (len < (size_t)n) {
        ++cand;
        uint64_t k = cand;

        /* Handle the special case k == 1 */
        if (k == 1) {
            if (len + 1 > cap) {
                cap = cap ? cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)1;
            continue;
        }

        /* Factor k and compute τ(k) */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t nf = 0;
        factor_u64(k, primes, exps, &nf);

        seq_t tau = 1;
        for (size_t i = 0; i < nf; ++i)
            tau *= (seq_t)(exps[i] + 1);

        /* Check the condition p <= τ(k) for each prime divisor */
        int ok = 1;
        for (size_t i = 0; i < nf && ok; ++i)
            if ((seq_t)primes[i] > tau)
                ok = 0;

        if (ok) {
            if (len + 1 > cap) {
                cap = cap ? cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
