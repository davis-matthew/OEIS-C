/*
 * A344033  a(n) = n * Sum_{d|n} d^mu(n/d).
 * https://oeis.org/A344033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} d^mu(n/d)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Small local Möbius function for uint64_t arguments. */
static int mobius_u64(uint64_t x)
{
    if (x == 1) return 1;
    int sign = 1;
    uint64_t t = x;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
                if (cnt > 1) return 0; /* square factor */
            }
            sign = -sign;
        }
    }
    if (t > 1) sign = -sign; /* remaining prime factor */
    return sign;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0 but avoid UB */

    uint64_t n_val = (uint64_t)n;
    seq_t n_seq = (seq_t)n_val;
    seq_t sum = (seq_t)0;

    /* Enumerate all divisors via trial division. */
    for (uint64_t d = 1; d <= n_val / d; ++d) {
        if (n_val % d == 0) {
            /* First divisor d. */
            uint64_t m = n_val / d;           /* m = n/d */
            int mu = mobius_u64(m);
            seq_t term;
            if (mu == 1) {
                term = n_seq * (seq_t)d;
            } else if (mu == -1) {
                term = n_seq / (seq_t)d;
            } else {
                term = n_seq;
            }
            sum += term;

            /* Second divisor n/d, if distinct. */
            uint64_t d2 = n_val / d;
            if (d2 != d) {
                uint64_t m2 = n_val / d2;   /* should be d */
                int mu2 = mobius_u64(m2);
                seq_t term2;
                if (mu2 == 1) {
                    term2 = n_seq * (seq_t)d2;
                } else if (mu2 == -1) {
                    term2 = n_seq / (seq_t)d2;
                } else {
                    term2 = n_seq;
                }
                sum += term2;
            }
        }
    }

    return sum;
}
