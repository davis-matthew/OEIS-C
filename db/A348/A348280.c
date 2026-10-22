/*
 * A348280  a(n) = Sum_{d|n} n^(d').
 * https://oeis.org/A348280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^(d') where d' = Σ_{p^k‖d} (k * p^(k-1)) * (d
 *            / p^k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (here Σ runs over the prime–power factors of d)     */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint.h"

#define MAX_PRIMES 16

/* Recursive enumeration of all divisors of n.  Each divisor is reconstructed
   from the prime factorisation of n.  For every divisor d the exponent d' is
   calculated from its prime factorisation and n^d' is added to *sum.  */
static void rec_div(int idx, int cnt,
                    uint64_t primes[], int exps[],
                    int cur_exps[], idx_t cur_div,
                    uint64_t n, seq_t *sum)
{
    if (idx == cnt) {
        idx_t dprime = 0;
        for (int i = 0; i < cnt; ++i) {
            if (cur_exps[i] == 0) continue;
            /* p_pow = prime[i]^cur_exps[i] */
            idx_t p_pow = 1;
            for (int t = 0; t < cur_exps[i]; ++t) p_pow *= primes[i];
            /* f = k * p^(k-1)  (k = cur_exps[i]) */
            idx_t f = cur_exps[i] * (p_pow / primes[i]);
            /* product of remaining prime-powers in the divisor */
            idx_t prod_other = cur_div / p_pow;
            dprime += f * prod_other;
        }
        seq_t term = pow_uint((uint64_t)n, dprime);
        *sum = *sum + term;
        return;
    }

    idx_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        cur_exps[idx] = e;
        rec_div(idx + 1, cnt, primes, exps, cur_exps,
                cur_div * val, n, sum);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    uint64_t primes[MAX_PRIMES];
    int     exps[MAX_PRIMES];
    int      cnt = 0;

    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    int      cur_exps[MAX_PRIMES] = {0};
    seq_t    total = 0;

    rec_div(0, cnt, primes, exps, cur_exps, 1, (uint64_t)n, &total);

    return total;
}
