/*
 * A336432  Number of ordered quadruples of divisors (d_i, d_j, d_k, d_m) of n
 *          such that GCD(d_i, d_j, d_k, d_m) > 1.
 * https://oeis.org/A336432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{∅≠T⊆P(n)} (-1)^{|T|+1} C(cnt_T,4), where P(n) is the
 *            set of prime factors of n, e_p is the exponent of p in n, and
 *            cnt_T = (∏_{p∈T} e_p) * (∏_{p∉T} (e_p+1))
 * Range:     u8 n <= 119, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle trivial cases */
    if (n < 2) return 0;

    uint64_t m = (uint64_t)n;
    uint64_t primes[8];
    uint64_t exps[8];
    size_t r = 0;

    /* Factor n by trial division */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) { m >>= 1; ++e; }
        primes[r] = 2; exps[r] = e; ++r;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[r] = p; exps[r] = e; ++r;
        }
    }
    if (m > 1) {
        primes[r] = m; exps[r] = 1; ++r;
    }

    seq_t result = 0;
    size_t subsets = 1u << r;

    for (size_t mask = 1; mask < subsets; ++mask) {
        /* count of divisors divisible by all primes in mask */
        seq_t cnt = 1;
        for (size_t i = 0; i < r; ++i) {
            if (mask & (1u << i))
                cnt *= (seq_t)exps[i];
            else
                cnt *= (seq_t)(exps[i] + 1);
        }

        /* binomial C(cnt,4) */
        seq_t binom = 0;
        if (cnt >= 4) {
            binom = (cnt * (cnt - 1) * (cnt - 2) * (cnt - 3)) / 24;
        }

        /* inclusion–exclusion sign */
        if (__builtin_popcountll(mask) & 1)
            result += binom;   /* odd |T| -> + */
        else
            result -= binom;   /* even |T| -> - */
    }

    return result;
}
