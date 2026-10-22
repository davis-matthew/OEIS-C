/*
 * A346121  Number of permutations of [n] whose order is a multiple of n.
 * https://oeis.org/A346121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of permutations of [n] whose order is a multiple
 *            of n
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses inclusion–exclusion on the prime powers dividing n and
 * a recurrence that counts permutations whose cycle lengths avoid a given
 * set of forbidden lengths. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Factorize n and store prime powers p^e */
    idx_t maxPrimes = 64;          /* enough for n up to 2^64-1 */
    idx_t primePow[maxPrimes];
    int primesCnt = 0;

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t pow = 1;
            while (temp % p == 0) {
                temp /= p;
                pow *= p;
            }
            primePow[primesCnt++] = pow;
        }
    }
    if (temp > 1) {
        primePow[primesCnt++] = temp;   /* remaining large prime */
    }

    /* Pre‑compute n! for possible use (needed for empty mask) */
    seq_t factN = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) factN *= (seq_t)i;

    seq_t sumEven = (seq_t)0;
    seq_t sumOdd  = (seq_t)0;

    /* Inclusion–exclusion over subsets of the distinct prime powers */
    idx_t masks = (idx_t)1 << primesCnt;
    for (idx_t mask = 0; mask < masks; ++mask) {
        /* Allocate array of allowed cycle lengths */
        bool *allowed = (bool *)calloc((size_t)(n + 1), sizeof(bool));
        for (idx_t i = 1; i <= n; ++i) allowed[i] = true;

        /* Apply the forbidden prime powers of this subset */
        for (int j = 0; j < primesCnt; ++j) {
            if (mask & ((idx_t)1 << j)) {
                idx_t m = primePow[j];
                for (idx_t i = m; i <= n; i += m) allowed[i] = false;
            }
        }

        /* DP array: f[k] = number of permutations of k elements
           whose cycle lengths are all allowed. */
        seq_t *f = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        f[0] = (seq_t)1;

        for (idx_t k = 1; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            seq_t factor = (seq_t)1;   /* (k-1)!/(k-l)! for current l */

            for (idx_t l = 1; l <= k; ++l) {
                if (l > 1) factor *= (seq_t)(k - l + 1);
                if (allowed[l]) {
                    sum += factor * f[k - l];
                }
            }
            f[k] = sum;
        }

        seq_t term = f[n];           /* number of permutations for this mask */
        int bits = __builtin_popcountll(mask);
        if ((bits & 1) == 0) sumEven += term;
        else                 sumOdd  += term;

        free(f);
        free(allowed);
    }

    return sumEven - sumOdd;
}
