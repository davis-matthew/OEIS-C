/*
 * A278639  Number of pairs of orientable necklaces with n beads and up to 3
 *          colors; i.e., turning the necklace over does not leave it
 *          unchanged. The turned-over necklace is not included in the count.
 * https://oeis.org/A278639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -(3^⌊(n+1)/2⌋ + 3^⌈(n+1)/2⌉)/4 + (1/(2n)) * Σ_{d|n} φ(d)
 *            * 3^(n/d) (n≥1) a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    const seq_t K = (seq_t)3;

    /* first term: -(K^floor((n+1)/2) + K^ceil((n+1)/2))/4 */
    idx_t f = (n + 1u) / 2u;          /* floor((n+1)/2) */
    idx_t c = (n + 2u) / 2u;          /* ceil((n+1)/2) */
    seq_t pow_f = big_pow(K, (seq_t)f);
    seq_t pow_c = big_pow(K, (seq_t)c);
    seq_t t1 = (pow_f + pow_c) / (seq_t)4;

    /* factor n to obtain its prime powers */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[16];
    int exp[16];
    size_t pcnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exp[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exp[pcnt] = 1;
        ++pcnt;
    }

    /* generate all divisors of n */
    size_t dcnt = 1;
    uint64_t divs[256];
    divs[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int e = exp[i];
        size_t cur = dcnt;
        uint64_t mult = 1;
        for (int ee = 1; ee <= e; ++ee) {
            mult *= p;
            for (size_t j = 0; j < cur; ++j) {
                divs[dcnt++] = divs[j] * mult;
            }
        }
    }

    /* sum over divisors */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        seq_t phi_d = phi((idx_t)d);                 /* use provided phi() */
        idx_t exp_nd = n / d;
        seq_t pow_nd = big_pow(K, (seq_t)exp_nd);
        sum += phi_d * pow_nd;
    }

    /* second term: (1/(2n)) * sum */
    seq_t two_n = (seq_t)(2 * (seq_t)n);
    seq_t t2 = sum / two_n;

    /* final result */
    seq_t result = t2 - t1;
    return result;
}
