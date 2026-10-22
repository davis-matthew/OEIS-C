/*
 * A307892  a(n) = lcm(tau(n), pod(n)) / n, where tau(k) = the number of
 *          divisors of k (A000005) and pod(n) = the product of divisors of k
 *          (A007955).
 * https://oeis.org/A307892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(tau(n), pod(n)) / n where tau(n) = number of divisors
 *            of n and pod(n) = product of all divisors of n The computation
 *            uses the identities pod(n) = n^(tau(n)/2) a(n) = (tau(n)/g) *
 *            n^((tau(n)/2)-1), where g is the product of the primes dividing
 *            tau(n) that also divide n. For tau(n) odd (...
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 89, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Helpers:   factor_u64, int_sqrt, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/factor_u64.h"
#include "../utils/int_sqrt.h"

/* Compute a(n) for n >= 1.  External linkage. */
seq_t a(idx_t n)
{
    /* Special handling for n == 1 */
    if (n == 1)
        return (seq_t)1;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute tau(n) = product of (exponent + 1) */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (exps[i] + 1);

    /* Factor tau(n) to find its prime exponents */
    idx_t tau_tmp = tau;
    uint64_t t_primes[32];
    idx_t t_exps[32];
    size_t t_cnt = 0;

    for (idx_t d = 2; d * d <= tau_tmp; ++d) {
        if (tau_tmp % d == 0) {
            idx_t e = 0;
            while (tau_tmp % d == 0) {
                tau_tmp /= d;
                ++e;
            }
            t_primes[t_cnt] = d;
            t_exps[t_cnt]   = e;
            ++t_cnt;
        }
    }
    if (tau_tmp > 1) {
        t_primes[t_cnt] = tau_tmp;
        t_exps[t_cnt]   = 1;
        ++t_cnt;
    }

    /* Compute g = product over primes p|tau(n) that also divide n of
       p^(exponent in tau(n)).  Since g <= tau, a 64‑bit integer is enough. */
    idx_t g = 1;
    for (size_t i = 0; i < t_cnt; ++i) {
        /* check if t_primes[i] divides n */
        for (size_t j = 0; j < cnt; ++j) {
            if (primes[j] == t_primes[i]) {
                /* multiply g by p^(exponent of tau) */
                for (idx_t k = 0; k < t_exps[i]; ++k)
                    g *= t_primes[i];
                break;
            }
        }
    }

    /* part1 = tau / g (integer) */
    idx_t part1 = tau / g;

    seq_t result;
    if (tau % 2 == 0) {
        /* tau even: exponent = tau/2 - 1 */
        idx_t exp = tau / 2 - 1;
        seq_t pow_val = seq_pow((seq_t)n, exp);
        result = seq_mul((seq_t)part1, pow_val);
    } else {
        /* tau odd: n is a perfect square.  Use sqrt(n)^(tau-2). */
        if (tau == 1)          /* only n == 1, already handled */
            result = (seq_t)1;
        else {
            idx_t sqrt_n = int_sqrt(n);
            seq_t pow_val = seq_pow((seq_t)sqrt_n, tau - 2);
            result = seq_mul((seq_t)part1, pow_val);
        }
    }

    return result;
}
