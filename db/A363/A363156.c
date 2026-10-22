/*
 * A363156  Expansion of Sum_{k>0} k * x^k / (1 - x^(5*k-2)).
 * https://oeis.org/A363156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Σ_{d | 5*n‑2, d ≡ 3 (mod 5)} (d+2)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void dfs_rec(int idx, idx_t cur, idx_t *primes, int *exps, int pf, seq_t *sum) {
    if (idx == pf) {
        if (cur % 5 == 3) {
            *sum += (seq_t)((cur + 2) / 5);
        }
        return;
    }
    idx_t p = primes[idx];
    idx_t val = 1;
    for (int k = 0; k <= exps[idx]; ++k) {
        dfs_rec(idx + 1, cur * val, primes, exps, pf, sum);
        val *= p;
    }
}

seq_t a(idx_t n) {
    /* Compute 5*n-2. For n up to 10 000 this fits into idx_t. */
    idx_t m = 5 * n - 2;

    /* Factorize m. */
    idx_t primes[64];
    int exps[64];
    int pf = 0;

    idx_t temp = m;

    /* Handle factor 2. */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        primes[pf] = 2;
        exps[pf] = e;
        ++pf;
    }

    /* Odd factors. */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pf] = p;
            exps[pf] = e;
            ++pf;
        }
    }

    if (temp > 1) {            /* Remaining prime factor. */
        primes[pf] = temp;
        exps[pf] = 1;
        ++pf;
    }

    /* Enumerate all divisors and accumulate the contribution. */
    seq_t sum = 0;
    dfs_rec(0, 1, primes, exps, pf, &sum);
    return sum;
}
