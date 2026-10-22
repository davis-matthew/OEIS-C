/*
 * A341319  Minimum number of cells needed in an n X n grid to win a majority
 *          of districts when all districts are required to be the same size.
 * https://oeis.org/A341319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{d | n^2} (floor(d/2)+1)*(floor(n^2/(2*d))+1)
 * Range:     u8 n <= 15, u16 n <= 509, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursively generate all divisors of n^2 and update the best product. */
static void dfs_div(int pos, seq_t curr, const int *primes, const int *exp2,
                    int pc, seq_t n2, seq_t *best)
{
    if (pos == pc) {
        seq_t t1 = curr / 2 + 1;
        seq_t t2 = n2 / ((seq_t)2 * curr) + 1;
        seq_t prod = t1 * t2;
        if (prod < *best)
            *best = prod;
        return;
    }
    seq_t mul = 1;
    for (int e = 0; e <= exp2[pos]; ++e) {
        dfs_div(pos + 1, curr * mul, primes, exp2, pc, n2, best);
        mul *= (seq_t)primes[pos];
    }
}

seq_t a(idx_t n)
{
    /* n^2 */
    seq_t n2 = (seq_t)n * (seq_t)n;

    /* Initial best: divisor = 1 gives value n2/2 + 1 */
    seq_t best = n2 / 2 + 1;

    /* Factorise n and build exponent array for n^2 (exponent doubled). */
    idx_t tmp = n;
    int primes[64];
    int exp2[64];
    int pc = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pc] = (int)p;
            exp2[pc] = e * 2; /* exponent in n^2 */
            ++pc;
        }
    }
    if (tmp > 1) {
        primes[pc] = (int)tmp;
        exp2[pc] = 2;        /* exponent 1 in n, 2 in n^2 */
        ++pc;
    }

    /* Enumerate all divisors of n^2. */
    dfs_div(0, (seq_t)1, primes, exp2, pc, n2, &best);

    return best;
}
