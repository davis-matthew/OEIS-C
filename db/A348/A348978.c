/*
 * A348978  Numerator of ratio A332993(n) / sigma(n).
 * https://oeis.org/A348978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A332993(n) / gcd(A000203(n), A332993(n))
 * Range:     u8 n <= 16, u16 n <= 4335, u32 n <= 9893, u64 n <= 9893,
 *            u128 n <= 9893, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = A332993(n) / A348977(n) = A332993(n) / gcd(sigma(n), A332993(n)) */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Smallest prime factor table (index 0..cap).  idx_t is an unsigned integer. */
static idx_t *spf = NULL;      /* spf[i] = smallest prime factor of i      */
static idx_t spf_cap = 0;      /* current sieve capacity (highest i stored) */

/* Build the SPF table up to and including lim.  All indices 0..lim are set. */
static void ensure_spf(idx_t lim)
{
    if (lim <= spf_cap)
        return;

    /* Allocate a new table of the required size.  The cast is required
       for C++ as malloc returns void*. */
    free(spf);
    spf = (idx_t *)malloc((lim + 1) * sizeof(idx_t));
    if (!spf) exit(1);          /* abort on allocation failure */

    spf_cap = lim;
    for (idx_t i = 0; i <= spf_cap; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    for (idx_t i = 2; i <= spf_cap; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            if (i > spf_cap / i) continue;   /* avoid overflow in the inner loop */
            for (idx_t j = i; j <= spf_cap / i; ++j) {
                idx_t k = i * j;
                if (spf[k] == 0) spf[k] = i;
            }
        }
    }
}

/* Compute A332993(n): the sum of successive largest proper divisors
   starting from n down to 1 inclusive. */
static seq_t chain_sum(idx_t n)
{
    seq_t sum = 0;
    idx_t cur = n;
    while (cur > 1) {
        sum += (seq_t)cur;
        cur = cur / spf[cur];            /* largest proper divisor */
    }
    sum += 1;                             /* include terminal 1 */
    return sum;
}

/* Compute sigma(n): sum of all positive divisors of n using its prime factors. */
static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;
    while (temp > 1) {
        uint64_t p = spf[temp];           /* p is a prime factor of temp */
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* Compute (p^(exp+1)-1)/(p-1) */
        seq_t p_pow = 1;
        for (int i = 0; i < exp + 1; ++i)
            p_pow *= (seq_t)p;

        seq_t term = (p_pow - 1) / (p - 1);
        result *= term;
    }
    return result;
}

/* Public function required by the specification.  Returns the numerator
   of A332993(n)/sigma(n), i.e., A332993(n)/gcd(A332993(n), sigma(n)). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0; harmless return */

    ensure_spf(n);

    seq_t chain = chain_sum(n);
    seq_t sigma = sigma_of(n);
    seq_t g     = gcd(chain, sigma);
    return chain / g;
}
