/*
 * A389464  Number of natural numbers in range [n .. sigma(n)-1] that are
 *          coprime to n.
 * https://oeis.org/A389464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k | n ≤ k ≤ σ(n)-1 , gcd(k,n)=1 }|, computed via
 *            inclusion–exclusion on the distinct prime factors of n
 * Range:     u8 n <= 431, u16 n <= 8147, u32 n <= 8147, u64 n <= 8147,
 *            u128 n <= 8147, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* Count integers X' (0 ≤ X' ≤ X) that are coprime to n, using
   inclusion–exclusion over the distinct prime divisors of n.  */
static seq_t count_coprime_upto(seq_t X, idx_t n)
{
    /* factor n into distinct primes */
    idx_t m = n;
    seq_t primes[16];
    int pcnt = 0;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            primes[pcnt++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) primes[pcnt++] = m;

    seq_t res = X;
    int subsets = 1 << pcnt;        /* 2^pcnt */
    for (int mask = 1; mask < subsets; ++mask) {
        seq_t prod = 1;
        int bits = 0;
        for (int i = 0; i < pcnt; ++i) {
            if (mask & (1 << i)) {
                prod *= primes[i];
                ++bits;
            }
        }
        seq_t term = X / prod;
        if (bits & 1)
            res -= term;
        else
            res += term;
    }
    return res;
}

/* OEIS sequence A389464: number of k in [n .. σ(n)-1] coprime to n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    seq_t sigma = sigma_of(n);           /* σ(n) */
    seq_t upper = sigma - 1;             /* σ(n)-1 */

    seq_t cop_upper = count_coprime_upto(upper, n);
    seq_t cop_before = count_coprime_upto(n - 1, n);

    return cop_upper - cop_before;
}
