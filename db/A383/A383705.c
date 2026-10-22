/*
 * A383705  Numerator of Dirichlet g.f.: Sum_{n>=1} a(n)/n^s = zeta(s)^(2/3).
 * https://oeis.org/A383705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, factorial, pow3, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A383705 – Numerator of Dirichlet g.f.: Sum_{n>=1} a(n)/n^s = zeta(s)^(2/3)
 *
 *  The sequence is multiplicative.  For a prime power p^k  (k ≥ 1) the
 *  Dirichlet coefficient is
 *
 *          binomial(2/3 + k - 1, k)
 *  =  ---------------------------------
 *      3^k * k!
 *
 *  where binomial(r,k) = r(r-1)...(r-k+1)/k!.
 *  The numerator of this rational number in lowest terms is
 *
 *          A(k) = numerator \left(\frac{ \prod_{i=0}^{k-1}(3i+2) }
 *                                     { 3^k\,k! } \right)
 *
 *  The function a(idx_t n) returns a(n) as a multiplicative product of
 *  the pre‑computed A(e) values for each prime exponent e of n.
 *
 *  The implementation factors n using the utility function
 *  factor_u64, pre‑computes A(k) for k as needed, and multiplies the
 *  results together.  All arithmetic is performed using the type
 *  seq_t.  The code works for any seq_t/idx_t definitions provided
 *  by the build system.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow3.h"
#include "../utils/factorial.h"
#include "../utils/seq_gcd.h"

static seq_t *a_prime_power = NULL;   /* a_prime_power[k] == A(k) */
static size_t   a_prime_power_size = 0;   /* number of stored elements */

/*  Ensure that a_prime_power[e] is computed for all 0 ≤ e ≤ k.  */
static void ensure_a_prime_power(idx_t k)
{
    if ((size_t)k < a_prime_power_size)
        return;

    size_t new_size = (size_t)k + 1;
    seq_t *new_arr = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_arr) return; /* malloc failure – caller will multiply by 0 later */

    /* copy old values if any */
    for (size_t i = 0; i < a_prime_power_size; ++i)
        new_arr[i] = a_prime_power[i];

    /* compute missing values */
    for (size_t i = a_prime_power_size; i < new_size; ++i) {
        if (i == 0) {
            new_arr[0] = (seq_t)1;  /* A(0) = 1, not used but convenient */
            continue;
        }
        idx_t e = (idx_t)i;
        seq_t prod = (seq_t)1;
        for (idx_t j = 0; j < e; ++j)
            prod *= (seq_t)(3 * j + 2);

        seq_t den = pow3(e);
        seq_t kfac = factorial(e);
        den *= kfac;

        seq_t g = seq_gcd(prod, den);
        new_arr[i] = prod / g;     /* numerator after reduction */
    }

    free(a_prime_power);
    a_prime_power = new_arr;
    a_prime_power_size = new_size;
}

/*  Public function required by the test harness.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for index 0; return 0 */

    uint64_t nn   = (uint64_t)n;   /* factor_u64 expects uint64_t */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t   cnt;

    factor_u64(nn, pr, exp, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exp[i];
        ensure_a_prime_power(e);
        res *= a_prime_power[e];
    }
    return res;
}
