/*
 * A378227  XOR-Moebius transform of A318467, where A318467(n) = 2*n XOR
 *          sigma(n).
 * https://oeis.org/A378227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = XOR_{d | n, d squarefree} ( 2*(n/d) XOR σ(n/d) ), where
 *            σ(m) is the sum of divisors of m. The XOR-Moebius transform of
 *            A318467. The function factorises n once and enumerates all
 *            square‑free divisors via subsets of the distinct prime factors
 * Range:     u8 n <= 16, u16 n <= 490, u32 n <= 9647, u64 n <= 9647,
 *            u128 n <= 9647, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Factor n completely: primes[] holds the distinct primes,
       exps[] stores their exponents in n, and cnt the number of
       distinct prime factors.  factor_u64 returns zeros in exps
       if exponents are 1; the function always produces the correct
       multiset. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;           /* XOR accumulator */

    /* Enumerate all subsets of the distinct primes.  Each subset
       corresponds to a square‑free divisor d of n.  For every
       subset we compute
          e   = n/d,
          σ(e) = product over p|e of (p^(k+1)-1)/(p-1),
       where k is the exponent of p in e.  Then we XOR
          2*e XOR σ(e)  into the result. */
    const size_t max_mask = (size_t)1 << cnt;
    for (size_t mask = 0; mask < max_mask; ++mask) {
        seq_t sigma_factor = 1;   /* product of σ contributions */
        seq_t prod_primes = 1;    /* product of primes in subset d */

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p  = primes[i];
            int       k  = (int)exps[i];   /* exponent of p in n */

            if (mask & ((size_t)1 << i)) {
                /* Prime p is in d, so it is removed from e
                   (exponent in e decreases by 1). */
                k -= 1;
                prod_primes *= (seq_t)p;      /* d contains p once */
            }
            /* σ contribution for prime p in e: (p^(k+1)-1)/(p-1) */
            seq_t num = pow_seq((seq_t)p, k + 1);
            seq_t den = (seq_t)(p - 1);
            sigma_factor *= (num - 1) / den;
        }

        seq_t e_value = (seq_t)n / prod_primes;     /* e = n/d   */
        seq_t val = ((seq_t)2 * e_value) ^ sigma_factor;
        result ^= val;
    }

    return result;
}
