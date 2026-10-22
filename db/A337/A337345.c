/*
 * A337345  Number of divisors d of n for which A003961(d) > 2*d, where
 *          A003961 is fully multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A337345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of divisors d of n for which A003961(d) > 2*d
 *            where A003961 is fully multiplicative with a(p) = nextprime(p)
 * Range:     u8 n <= 55, u16 n <= 7008, u32 n <= 7008, u64 n <= 7008,
 *            u128 n <= 7008, big any n
 * Helpers:   build_primes_upto, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factor n, enumerate all its divisors using the prime
   exponents, compute for each divisor the multiplicative value
   A003961(d) = ∏ nextprime(p)^e, and count the divisors satisfying
   the inequality.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* Return the smallest prime larger than p.  */
static seq_t _next_prime(uint64_t p)
{
    uint64_t q = p + 1;
    while (!is_prime((idx_t)q))
        ++q;
    return (seq_t)q;
}

/* Recursively enumerate all divisor exponents of n.  */
static void _enumerate(const uint64_t *pr, const uint64_t *ex,
                       size_t plen, size_t pos,
                       seq_t cur_d, seq_t cur_a,
                       seq_t *cnt)
{
    if (pos == plen) {
        if (cur_a > 2 * cur_d)
            ++(*cnt);
        return;
    }
    uint64_t prime = pr[pos];
    uint64_t emax   = ex[pos];
    seq_t nextp = _next_prime(prime);
    seq_t d_local = cur_d;
    seq_t a_local = cur_a;
    for (uint64_t e = 0; e <= emax; ++e) {
        _enumerate(pr, ex, plen, pos + 1, d_local, a_local, cnt);
        if (e == emax) break;
        d_local *= (seq_t)prime;
        a_local *= nextp;
    }
}

/* Main function required by the build system.  */
seq_t a(idx_t n)
{
    /* One-time prime table for nextprime lookup.  */
    static uint64_t *g_primes = NULL;
    static size_t g_prime_cnt = 0;

    if (g_primes == NULL) {
        build_primes_upto(20000, &g_primes, &g_prime_cnt);
    }

    /* Factor n into primes and exponents.  */
    uint64_t n_val = (uint64_t)n;
    uint64_t primes_n[16];
    uint64_t exps_n[16];
    size_t cnt_n = 0;
    factor_u64(n_val, primes_n, exps_n, &cnt_n);

    /* Count divisors satisfying the condition.  */
    seq_t count = 0;
    _enumerate(primes_n, exps_n, cnt_n, 0, (seq_t)1, (seq_t)1, &count);

    return count;
}
