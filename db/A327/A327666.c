/*
 * A327666  a(n) = Sum_{k = 1..n} (-1)^(Omega(k) - omega(k)), where Omega(k)
 *          counts prime factors of k with multiplicity and omega(k) counts
 *          distinct prime factors.
 * https://oeis.org/A327666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(Omega(k)-omega(k))
 * Range:     u8 n <= 519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a sieve for smallest prime factors to compute each term in O(log n) and
   accumulates the partial sums in a signed 64‑bit integer.                */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global array of smallest prime factors.  The array is
   grown lazily to accommodate the largest value of n requested.   */
static idx_t *spf = NULL;      /* smallest prime factor of each i (spf[i]==i if i is prime) */
static idx_t spf_len = 0;      /* current capacity of spf (number of valid indices: 0..spf_len-1) */

/* Grow the spf array to hold indices up to new_len-1.  The sieve
   algorithm below runs in O(new_len log log new_len).                */
static void grow_spf(idx_t new_len)
{
    if (new_len <= spf_len)
        return;

    /* realloc with an explicit cast for C++ compatibility */
    spf = (idx_t *)realloc(spf, new_len * sizeof(idx_t));
    /* initialize the new slots with their own indices */
    for (idx_t i = spf_len; i < new_len; ++i)
        spf[i] = i;

    /* run the sieve on the new portion */
    for (idx_t i = 2; i * i < new_len; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            for (idx_t j = i * i; j < new_len; j += i) {
                if (spf[j] == j)                /* first time we touch j */
                    spf[j] = i;
            }
        }
    }
    spf_len = new_len;
}

/* Main sequence function as required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* safe guard – undefined for n<1 in OEIS */

    grow_spf(n + 1);                          /* ensure spf[1..n] are ready */

    long long acc = 0;                        /* signed accumulator for ±1 terms */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t x = k;
        int even_exp_parity = 0;              /* parity of count of primes with even exponent */

        /* factor k using the precomputed spf array */
        while (x > 1) {
            idx_t p = spf[x];
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if ((exp & 1) == 0)               /* exp is even -> contributes sign -1 */
                even_exp_parity ^= 1;
        }

        /* sign is +1 if even_exp_parity==0 else -1 */
        acc += (even_exp_parity == 0) ? 1 : -1;
    }

    return (seq_t)acc;                       /* cast to the required seq_t type */
}
