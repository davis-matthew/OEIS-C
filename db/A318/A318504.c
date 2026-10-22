/*
 * A318504  SumXOR of divisors of n, up to, but not including the second
 *          largest of them A032742(n); a(1) = 0 by convention.
 * https://oeis.org/A318504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A032742(n) XOR A227320(n) a(1) = 0 by convention. The
 *            function returns the XOR of all divisors of n except the second
 *            largest (A032742(n)) and the largest (n)
 * Range:     u8 n <= 767, u16 n <= 9421, u32 n <= 9421, u64 n <= 9421,
 *            u128 n <= 9421, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Recursively generate all divisors of n and accumulate their XOR. */
static void
gen_divisors(size_t idx, seq_t cur, const uint64_t *primes,
             const uint64_t *exps, size_t pcnt, seq_t *xor_acc)
{
    if (idx == pcnt) {
        *xor_acc ^= cur;
        return;
    }
    seq_t val = cur;
    for (unsigned e = 0; e <= (unsigned)exps[idx]; ++e) {
        gen_divisors(idx + 1, val, primes, exps, pcnt, xor_acc);
        val = val * (seq_t)primes[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)                /* a(1) = 0 by convention */
        return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t m = nn;

    /* Factor n and find its smallest prime factor. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    uint64_t spf = 0;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            if (!spf) spf = p;
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt]  = p;
            exps[pcnt]    = e;
            ++pcnt;
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        if (!spf) spf = m;
        primes[pcnt]  = m;
        exps[pcnt]    = 1;
        ++pcnt;
    }

    /* Second largest divisor is n / spf.   */
    seq_t secondLargest = (seq_t)(nn / spf);

    /* XOR of all divisors */
    seq_t xor_all = 0;
    gen_divisors(0, (seq_t)1, primes, exps, pcnt, &xor_all);

    /* Exclude the second largest and the largest divisor. */
    seq_t result = xor_all ^ secondLargest ^ (seq_t)nn;
    return result;
}
