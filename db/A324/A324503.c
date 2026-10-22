/*
 * A324503  a(n) = numerator of Sum_{d|n} (d/tau(d)) where tau(k) = the number
 *          of divisors of k (A000005).
 * https://oeis.org/A324503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} (d/tau(d)) where tau(k) is the
 *            number of divisors of k
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   factor_u64, pow_seq, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/pow_seq.h"     /* seq_t pow_seq(seq_t a, int k); */
#include "../utils/seq_gcd.h"     /* seq_t seq_gcd(seq_t a, seq_t b); */

/* helper: add a rational d/t to the current fraction num/den,
   storing the reduced result back into num/den */
static inline void add_frac(seq_t *num, seq_t *den, seq_t d, seq_t t)
{
    seq_t new_num = (*num) * t + d * (*den);
    seq_t new_den = (*den) * t;
    seq_t g = seq_gcd(new_num, new_den);
    *num = new_num / g;
    *den = new_den / g;
}

/* recursive generation of all divisors of n together with their tau-values */
static void dfs_divisors(int idx, int cnt,
                         const uint64_t *pr, const uint64_t *ex,
                         seq_t cur, seq_t tau_cur,
                         seq_t *num, seq_t *den)
{
    if (idx == cnt) {
        /* cur is a divisor d, tau_cur is tau(d) */
        add_frac(num, den, cur, tau_cur);
        return;
    }

    seq_t p = (seq_t)pr[idx];
    seq_t pow_p = 1;                /* p^f */
    for (int f = 0; f <= (int)ex[idx]; ++f) {
        seq_t new_cur = cur * pow_p;
        seq_t new_tau = tau_cur * (f + 1);
        dfs_divisors(idx + 1, cnt, pr, ex, new_cur, new_tau, num, den);
        pow_p = pow_p * p;          /* next power */
    }
}

seq_t a(idx_t n)
{
    /* factor n */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* start with zero fraction 0/1 */
    seq_t num = 0;
    seq_t den = 1;

    /* generate all divisors and accumulate the sum */
    dfs_divisors(0, (int)cnt, pr, ex, 1, 1, &num, &den);

    /* the fraction is reduced; the numerator is the required value */
    return num;
}
