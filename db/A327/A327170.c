/*
 * A327170  Number of divisors d of n such that A327171(d) (= phi(d)*core(d))
 *          is equal to n.
 * https://oeis.org/A327170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that core(d)*phi(d) = n,
 *            where core(d) is the product of the primes dividing d with odd
 *            exponent (the square‑free part of d)
 * Range:     u8 n <= 9854, u16 n <= 9854, u32 n <= 9854, u64 n <= 9854,
 *            u128 n <= 9854, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static void dfs(const uint64_t *primes, const int *exps, size_t pcnt,
                size_t idx, seq_t phi, seq_t core, seq_t nseq, seq_t *cnt)
{
    if (idx == pcnt) {
        if (phi * core == nseq)
            (*cnt)++;
        return;
    }
    uint64_t p   = primes[idx];
    int      maxe = exps[idx];
    seq_t pow_p = (seq_t)1;               /* p^e   */

    for (int e = 0; e <= maxe; ++e) {
        seq_t phi_factor, core_factor;

        if (e == 0) {
            phi_factor = (seq_t)1;
            core_factor = (seq_t)1;
        } else {
            pow_p = pow_p * (seq_t)p;          /* p^e             */
            seq_t pow_e_minus1 = pow_p / (seq_t)p;  /* p^(e-1) */
            phi_factor = pow_e_minus1 * ((seq_t)p - (seq_t)1);
            core_factor = (e & 1) ? (seq_t)p : (seq_t)1;
        }

        seq_t new_phi = phi * phi_factor;
        seq_t new_core = core * core_factor;

        dfs(primes, exps, pcnt, idx + 1, new_phi, new_core, nseq, cnt);
    }
}

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* Factorise n.  For the sizes relevant to the tests this simple
       trial division is sufficient. */
    uint64_t primes[64];
    int      exps[64];
    size_t   pcnt = 0;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    seq_t count = (seq_t)0;
    dfs(primes, exps, pcnt, (size_t)0, (seq_t)1, (seq_t)1, nseq, &count);
    return count;
}
