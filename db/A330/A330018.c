/*
 * A330018  a(n) = Sum_{d|n} (bigomega(d) - omega(d)).
 * https://oeis.org/A330018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (bigomega(d) - omega(d))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"

/* Small static cache for primes */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Ensure that 'primes' contains all primes ≤ limit */
static void ensure_primes_upto(idx_t limit)
{
    if (primes == NULL || primes_limit < limit) {
        if (primes != NULL)
            free(primes);
        size_t cnt;
        build_primes_upto(limit, &primes, &cnt);
        primes_cnt = cnt;
        primes_limit = limit;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 */
        return 0;

    idx_t temp = n;
    idx_t lim = int_sqrt(temp);
    ensure_primes_upto(lim);

    seq_t tau = 1;        /* divisor count of the processed part */
    seq_t a_val = 0;      /* current value of a for the processed part */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p > lim)
            break;
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pe_tau = (seq_t)(e + 1);
            seq_t pe_a   = ((seq_t)e * (seq_t)(e - 1)) / 2;   /* e*(e-1)/2 */

            seq_t new_a = tau * pe_a + a_val * pe_tau;
            tau = tau * pe_tau;
            a_val = new_a;

            if (temp == 1)
                break;
            lim = int_sqrt(temp);
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        seq_t pe_tau = 2;            /* e = 1 */
        seq_t pe_a   = 0;            /* e*(e-1)/2 = 0 */
        seq_t new_a = tau * pe_a + a_val * pe_tau;
        a_val = new_a;
    }

    return a_val;
}
