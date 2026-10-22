/*
 * A355875  For n >= 1, k >= 1; a(n) = Sum_{k=1..n} f(n,k). f(n,k) = n*k/(n +
 *          k) if n*k divides (n+k), 0 otherwise.
 * https://oeis.org/A355875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n*k/(n+k)) if (n+k)|(n*k), else 0
 * Range:     u8 n <= 15, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursively generate all divisors of n^2 and accumulate contributions that satisfy n < m ≤ 2n. */
static void gen_divs(const uint64_t *primes, const uint8_t *exps, size_t idx, size_t num,
                     seq_t curr, seq_t n, seq_t n2, seq_t *sum)
{
    if (idx == num) {
        if (curr > n && curr <= ((n) << 1)) {   /* m in (n, 2n] */
            seq_t t = n2 / curr;                 /* t = n^2 / m  (integer division) */
            *sum += n - t;                       /* contribution = n - t */
        }
        return;
    }
    seq_t mult = 1;
    for (uint8_t e = 0; e <= 2 * exps[idx]; ++e) {
        gen_divs(primes, exps, idx + 1, num, curr * mult, n, n2, sum);
        mult *= primes[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;

    /* Factor n into primes (trial division, fast for n ≤ 10 000). */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes_local[12];
    uint8_t   exps_local[12];
    size_t    num = 0;

    if (tmp % 2 == 0) {
        uint8_t e = 0;
        while (tmp % 2 == 0) { tmp /= 2; ++e; }
        primes_local[num] = 2; exps_local[num] = e; ++num;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes_local[num] = p; exps_local[num] = e; ++num;
        }
    }
    if (tmp > 1) {
        primes_local[num] = tmp; exps_local[num] = 1; ++num;
    }

    /* Compute n^2 and sum over suitable divisors. */
    seq_t n2 = ns * ns;
    seq_t sum = 0;
    gen_divs(primes_local, exps_local, 0, num, (seq_t)1, ns, n2, &sum);

    return sum;
}
