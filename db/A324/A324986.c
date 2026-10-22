/*
 * A324986  a(n) = Sum_{d|n} (tau(d)*sigma(d)) where tau(k) = the number of
 *          divisors of k (A000005) and sigma(k) = the sum of the divisors of
 *          k (A000203).
 * https://oeis.org/A324986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} tau(d)*sigma(d) where tau(d) = number of
 *            divisors of d, sigma(d) = sum of divisors of d. The function
 *            implements this formula using the multiplicative property:
 *            a(p^e) = Σ_{k=0..e} (p^(k+1)-1)*(k+1)/(p-1) and a(n) = ∏ a(p^e)
 *            for the prime factorisation n = ∏ p^e
 * Range:     u8 n <= 16, u16 n <= 478, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* --- lightweight factorisation routine ---------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c]   = n;
        ex[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* --- sequence definition ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1u) return 0;          /* undefined for n < 1, but safe guard */

    uint64_t primes[64];
    int      exps[64];
    size_t   cnt;

    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];

        seq_t sum = 0;
        seq_t p_power = 1;  /* will hold p^(k+1) inside the loop */

        for (int k = 0; k <= e; ++k) {
            p_power *= (seq_t)p;                     /* p^(k+1) */
            seq_t num   = p_power - 1;               /* p^(k+1) - 1 */
            seq_t term  = (num * (seq_t)(k + 1)) / (p - 1);
            sum += term;
        }

        result *= sum;
    }

    return result;
}
