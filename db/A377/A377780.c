/*
 * A377780  First differences of A000015 (smallest prime-power >= n).
 * https://oeis.org/A377780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n+1) - b(n) where b(n) is the least prime power ≥ n. We
 *            include a special case a(1)=0 since the first difference is not
 *            defined
 * Range:     u8 n <= 31, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/build_primes_upto.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static storage for primes up to a current limit */
static uint64_t *prime_list = NULL;
static size_t prime_list_cnt = 0;
static idx_t prime_list_limit = 0;

/* Ensure that prime_list contains all primes ≤ limit.  The
   helper build_primes_upto allocates the array. */
static void ensure_primes(idx_t limit)
{
    if (prime_list_limit >= limit)
        return;
    if (prime_list != NULL)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_list_cnt);
    prime_list_limit = limit;
}

/* Return the smallest prime power ≥ n.  For n ≤ 1 the function returns 1. */
static seq_t next_prime_power_ge(seq_t n)
{
    if (n <= (seq_t)1)
        return (seq_t)1;            /* A000015(1) = 1 by convention */

    /* By Bertrand's postulate there is a prime p with n < p < 2*n;
       generate primes up to 2*n + 10 to be safe. */
    idx_t lim = (idx_t)n;
    lim = lim * 2 + 10;
    ensure_primes(lim);

    seq_t best = 0;                 /* 0 is used as “not set yet” */
    for (size_t i = 0; i < prime_list_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t val = (seq_t)p;

        /* If val already ≥ n, we have a candidate without powers. */
        if (val >= n) {
            if (best == 0 || val < best)
                best = val;
            continue;
        }

        /* Compute successive powers of p until we reach ≥ n. */
        while (val < n) {
            val *= (seq_t)p;
            if (best != 0 && val >= best)
                break;              /* This power cannot beat the current best */
        }
        if (best == 0 || val < best)
            best = val;
    }
    return best;                     /* best ≥ n and minimal */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;             /* defined special case */
    seq_t p_n  = next_prime_power_ge((seq_t)n);
    seq_t p_n1 = next_prime_power_ge((seq_t)(n + 1));
    return p_n1 - p_n;
}
