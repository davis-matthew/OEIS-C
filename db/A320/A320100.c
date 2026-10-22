/*
 * A320100  Automata sum similar to A102376 but using mod 5.
 * https://oeis.org/A320100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k1=0..n} sum_{k2=0..n} (binom(n,k1) mod 5) *
 *            (binom(n,k2) mod 5) mod 5
 * Range:     u8 n <= 12, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the fact that the raw value at cell (x,y) in the
   CA after n generations is binom(n,(n-x)/2) * binom(n,(n-y)/2),
   and the total sum of values after mod 5 is simply the sum of
   binom(n,k) mod 5 over all k, and their pairwise products.  */

#include <stddef.h>
#include <stdint.h>

static const int binom_small[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,2,1,0,0},
    {1,3,3,1,0},
    {1,4,1,4,1}
};

seq_t a(idx_t n)
{
    /* convert idx_t to a portable unsigned integer */
    unsigned long long N = (unsigned long long)n;
    uint64_t count[5] = {0,0,0,0,0};

    /* compute binomial(n,k) mod 5 for all k and count residues */
    for (unsigned long long k = 0; k <= N; ++k) {
        unsigned long long tN = N, tK = k;
        int res = 1;
        while (tN || tK) {
            int ni = (int)(tN % 5);
            int ki = (int)(tK % 5);
            if (ki > ni) { res = 0; break; }
            res = (res * binom_small[ni][ki]) % 5;
            tN /= 5;
            tK /= 5;
        }
        count[res]++;
    }

    /* accumulate the sum of pairwise products of residues modulo 5 */
    seq_t sum = 0;
    for (int a = 0; a < 5; ++a) {
        for (int b = 0; b < 5; ++b) {
            seq_t term = (seq_t)count[a] * (seq_t)count[b];
            int prod_mod = (a * b) % 5;
            term *= (seq_t)prod_mod;
            sum += term;
        }
    }
    return sum;
}
