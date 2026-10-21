/*
 * A281785  a(n) is multiplicative with a(2^e) = 1, a(3^e) = -8 if e>0, a(p^e)
 *          = (p^(e+1) - 1) / (p - 1) if p>3.
 * https://oeis.org/A281785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign
 * Formula:   not stated
 * Range:     i8 n <= 50, i16 n <= 8084, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A281785 – multiplicative function.
    a(2^e)   = 1
    a(3^e)   = -8  (for e > 0)
    a(p^e)   = (p^(e+1)-1)/(p-1)  for prime p > 3
    A(n) = product of the values for each prime power in the factorisation of n.
    a(1) = 1.  The function is valid for every n ≥ 1.  */

#include <stdint.h>
#include <stddef.h>

static void factor(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            uint64_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[*cnt] = d;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[*cnt] = n;
        exps[*cnt]   = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    factor((uint64_t)n, primes, exps, &cnt);

    long long result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];
        long long contrib;

        if (p == 2) {
            contrib = 1;                   /* a(2^e) = 1 */
        } else if (p == 3) {
            contrib = -8;                  /* a(3^e) = -8 for any e>0 */
        } else {
            long long sum = 1;              /* geometric series 1 + p + p^2 + ... + p^e */
            long long pow = 1;
            for (unsigned j = 0; j < e; ++j) {
                pow  *= (long long)p;
                sum  += pow;
            }
            contrib = sum;
        }

        result *= contrib;
    }

    return (seq_t)result;
}
