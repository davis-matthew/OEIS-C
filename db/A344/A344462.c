/*
 * A344462  Number of pairs (d1,d2) of divisors of n such that d1 < d2 and
 *          d2^2 + d1*(n - d2)^2 = n^2.
 * https://oeis.org/A344462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (d1,d2) of divisors of n with d1 < d2 and
 *            d2^2 + d1·(n - d2)^2 = n^2
 * Range:     u8 n <= 23, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recursively generate all divisors from the prime factorisation. */
static void gen_divs_rec(const idx_t primes[], const unsigned exps[],
                         int pc, int idx, seq_t cur, seq_t divs[],
                         int *divcnt)
{
    if (idx == pc) {
        divs[(*divcnt)++] = cur;
        return;
    }
    idx_t p = primes[idx];
    seq_t curpow = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(primes, exps, pc, idx + 1, cur * curpow, divs, divcnt);
        curpow *= (seq_t)p;
    }
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* sequence starts at n=1 */

    /* 1. Factor n. */
    idx_t m = n;
    idx_t primes[20];
    unsigned exps[20];
    int pc = 0;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (m > 1) {
        primes[pc] = m;
        exps[pc] = 1;
        ++pc;
    }

    /* 2. Generate all divisors. */
    seq_t divs[256];
    int divcnt = 0;
    gen_divs_rec(primes, exps, pc, 0, 1, divs, &divcnt);

    /* 3. Count qualifying pairs. */
    seq_t sn = (seq_t)n;
    seq_t nsq = sn * sn;
    seq_t count = 0;

    for (int i = 0; i < divcnt; ++i) {
        seq_t d1 = divs[i];
        for (int j = 0; j < divcnt; ++j) {
            seq_t d2 = divs[j];
            if (d1 >= d2) continue;          /* enforce d1 < d2 */
            seq_t term = (d2 * d2) + d1 *
                         ((sn - d2) * (sn - d2));
            if (term == nsq) {
                ++count;
            }
        }
    }

    return count;
}
