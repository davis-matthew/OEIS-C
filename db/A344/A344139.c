/*
 * A344139  a(n) = Sum_{x_1|n, x_2|n, x_3|n, x_4|n, x_5|n}
 *          gcd(x_1,x_2,x_3,x_4,x_5).
 * https://oeis.org/A344139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} φ(n/d) * τ(d)^5
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, pow5_unsigned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes smallest prime factors up to the
   maximum n requested.  It works for both C (where seq_t is an
   integral type) and C++ (where seq_t can be an arbitrary‐precision
   integer class) builds.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"   /* build_spf(limit, &array) */
#include "../utils/pow5_unsigned.h"

/* Static cache of smallest prime factors */
static idx_t *spf = NULL;          /* array of size spf_limit+1 */
static idx_t spf_limit = 0;

/* Ensure the SPF array is built up to at least 'limit' */
static void build_spf_for(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    idx_t *ptr = NULL;
    build_spf(limit, &ptr);          /* Utility fills 'ptr' with array[limit+1] */
    if (spf)
        free(spf);
    spf = ptr;
    spf_limit = limit;
}

/* φ(m) for a 64‑bit integer, using the global SPF table */
static seq_t phi_uint64(uint64_t m)
{
    seq_t result = (seq_t)m;
    while (m > 1) {
        uint64_t p = spf[m];
        if (p == 0)          /* prime left uninitialized in SPF */
            p = m;
        unsigned exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        result = result / (seq_t)p * (seq_t)(p - 1);
    }
    return result;
}

/* τ(m) for a 64‑bit integer, using the global SPF table */
static unsigned tau_uint64(uint64_t m)
{
    unsigned result = 1;
    while (m > 1) {
        uint64_t p = spf[m];
        if (p == 0)
            p = m;
        unsigned exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        result *= (exp + 1);
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0;

    /* Build SPF up to n once */
    build_spf_for(n);

    seq_t result = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        /* First divisor d */
        seq_t phi_val = phi_uint64(n / d);
        unsigned tau_val = tau_uint64(d);
        seq_t term = phi_val * pow5_unsigned(tau_val);
        result += term;

        /* Second divisor e = n/d if distinct */
        idx_t e = n / d;
        if (e != d) {
            seq_t phi_val2 = phi_uint64(n / e);   /* which is phi(d) */
            unsigned tau_val2 = tau_uint64(e);
            seq_t term2 = phi_val2 * pow5_unsigned(tau_val2);
            result += term2;
        }
    }

    return result;
}
