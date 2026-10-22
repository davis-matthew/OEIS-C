/*
 * A324932  Numerator in the division of n by the product of prime indices of
 *          n.
 * https://oeis.org/A324932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, ∏_{p^e∥n} index(p)^e)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64, gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  include necessary helper headers  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"

static uint64_t *primes = NULL;      /* array of primes */
static size_t primes_cnt = 0;        /* number of primes stored */

/* extend the primes array to contain all primes ≤ limit */
static void ensure_primes_upto(idx_t limit)
{
    if (primes_cnt > 0 && primes[primes_cnt - 1] >= limit)
        return;
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
}

/* binary search to find the 1‑based index of prime p */
static idx_t get_prime_index(uint64_t p)
{
    ensure_primes_upto(p);
    idx_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (primes[mid] == p)
            return mid + 1;       /* indices in OEIS start at 1 */
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* should never happen for a valid prime */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* not used, but just in case */
        return (seq_t)0;

    /* factorize n */
    uint64_t fac[64];
    uint64_t exp[64];
    size_t factor_cnt = 0;
    factor_u64(n, fac, exp, &factor_cnt);

    seq_t R = (seq_t)n;          /* numerator before reduction */
    seq_t P = (seq_t)1;          /* product of prime indices */

    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t prime = fac[i];
        idx_t power = (idx_t)exp[i];
        idx_t idx = get_prime_index(prime);
        seq_t idx_seq = (seq_t)idx;
        for (idx_t j = 0; j < power; ++j)
            P *= idx_seq;
    }

    seq_t g = gcd(R, P);
    return R / g;
}
