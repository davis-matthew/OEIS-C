/*
 * A253566  Permutation of natural numbers: a(n) = A243071(A122111(n)).
 * https://oeis.org/A253566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1}^{k-1} 2^{x_k - x_{k-i} + i - 1} where 2n =
 *            ∏_{j=1}^k prime(x_j) and prime(x_j) is the j‑th prime (prime
 *            indices counted from 1). This implements the definition of OEIS
 *            A253566
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Pre‑compute primes up to 20000 and their indices. */
    static bool done = false;
    static int  primes[20001];
    static int  prime_index[20001];
    static size_t prime_count = 0;

    if (!done) {
        bool is_comp[20001] = { false };
        const int LIMIT = 20000;
        for (int i = 2; i <= LIMIT; ++i) {
            if (!is_comp[i]) {
                primes[prime_count] = i;
                /* 1‑based index of prime i */
                prime_index[i] = (int)prime_count + 1;
                ++prime_count;
                if ((long long)i * i <= LIMIT) {
                    for (int j = i * i; j <= LIMIT; j += i)
                        is_comp[j] = true;
                }
            }
        }
        done = true;
    }

    /* Factor 2n and collect the prime indices. */
    idx_t v = (idx_t)2 * n;
    int idx_arr[64] = {0};
    int k = 0;                /* number of prime factors (with multiplicity) */

    for (size_t pi = 0; pi < prime_count &&
                       (idx_t)primes[pi] * primes[pi] <= v; ++pi) {
        int p = primes[pi];
        if (v % p == 0) {
            int times = 0;
            while (v % p == 0) {
                v /= p;
                ++times;
            }
            int pi_idx = prime_index[p];
            for (int t = 0; t < times; ++t)
                idx_arr[k++] = pi_idx;
        }
    }
    if (v > 1) {            /* remaining prime factor (≤20000) */
        int pi_idx = prime_index[(int)v];
        idx_arr[k++] = pi_idx;
    }

    /* Compute the required sum. */
    seq_t sum = 0;
    for (int i = 1; i < k; ++i) {
        int exponent = idx_arr[k - 1] - idx_arr[k - 1 - i] + i - 1;
        seq_t term = pow2((idx_t)exponent);
        sum += term;
    }

    return sum;
}
