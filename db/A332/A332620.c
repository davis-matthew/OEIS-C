/*
 * A332620  a(n) = Sum_{k=1..n} n^(n/gcd(n, k)).
 * https://oeis.org/A332620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * n^d
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where the sum runs over all positive divisors d of n.
 *  This is equivalent to the definition a(n) = Sum_{k=1..n} n^(n/gcd(n,k)). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Recursive helper to enumerate all divisors of n.
 * cur: current divisor,
 * phi_cur: current phi(partial divisor),
 * sum_ptr: accumulator for a(n). */
static void enum_div(idx_t pos, seq_t cur, seq_t phi_cur, idx_t n,
                      seq_t *sum_ptr,
                      const idx_t *primes, const int *exps, size_t cnt)
{
    if (pos == cnt) {
        /* term = phi(cur) * n^cur */
        seq_t term = phi_cur * big_pow((seq_t)n, (seq_t)cur);
        *sum_ptr += term;
        return;
    }

    idx_t p = primes[pos];
    seq_t power = 1;   /* p^k   */
    seq_t part  = 1;   /* phi(p^k) term for current k */

    for (int k = 0; k <= exps[pos]; ++k) {
        seq_t new_cur  = cur * power;
        seq_t new_phi  = phi_cur * part;

        enum_div(pos + 1, new_cur, new_phi, n, sum_ptr,
                 primes, exps, cnt);

        if (k == exps[pos]) break;  /* last k: nothing more to do */

        power = power * (seq_t)p;   /* next p^k */

        if (k == 0)
            part = (seq_t)(p - 1);   /* phi(p^1) = p-1 */
        else
            part = part * (seq_t)p;  /* phi(p^k) = (p-1)*p^(k-1) */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    /* Factor n into primes and exponents.
     * The factor_u64 routine populates primes_raw and exps_raw. */
    uint64_t primes_raw[64];
    uint64_t exps_raw[64];
    size_t    cnt = 0;
    factor_u64((uint64_t)n, primes_raw, exps_raw, &cnt);

    /* Convert to idx_t / int arrays for recursion. */
    idx_t  primes[64];
    int    exps[64];
    for (size_t i = 0; i < cnt; ++i) {
        primes[i] = (idx_t)primes_raw[i];
        exps[i]   = (int)exps_raw[i];
    }

    seq_t sum = 0;
    enum_div(0, (seq_t)1, (seq_t)1, n, &sum,
             primes, exps, cnt);

    return sum;
}
