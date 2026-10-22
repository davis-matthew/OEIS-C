/*
 * A372709  a(n) = Sum_{k=1..n} tau(n*k), where tau(n) = number of divisors of
 *          n, cf. A000005.
 * https://oeis.org/A372709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(n*k)
 * Range:     u8 n <= 19, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return 0;

    /* Allocate least prime factor table for numbers up to n */
    size_t N = (size_t)n;
    int *spf = (int *)malloc((N + 1) * sizeof(int));
    if (!spf) return 0; /* allocation failure – unlikely for small n */

    /* Initialize spf array */
    for (size_t i = 0; i <= N; ++i) spf[i] = (int)i;
    for (size_t i = 2; i * i <= N; ++i) {
        if (spf[i] == (int)i) {
            for (size_t j = i * i; j <= N; j += i)
                if (spf[j] == (int)j) spf[j] = (int)i;
        }
    }

    /* Factorize n: store its prime factors and exponents */
    int primes_n[16];  /* n <= 10^4, so at most 16 distinct primes */
    int exps_n[16];
    int prcnt = 0;
    size_t temp = n;
    while (temp > 1) {
        int p = spf[(int)temp];
        int cnt = 0;
        while (temp % p == 0) {
            temp /= (size_t)p;
            ++cnt;
        }
        primes_n[prcnt] = p;
        exps_n[prcnt]  = cnt;
        ++prcnt;
    }

    /* Accumulate the sum of tau(n*k) for k=1..n */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t tau = 1;
        size_t tmp = (size_t)k;

        /* Handle primes shared with n */
        for (int i = 0; i < prcnt; ++i) {
            int p = primes_n[i];
            int cnt = 0;
            while (tmp % (size_t)p == 0) {
                tmp /= (size_t)p;
                ++cnt;
            }
            tau *= ((seq_t)exps_n[i] + (seq_t)cnt + 1);
        }

        /* Handle remaining prime factors not in n */
        while (tmp > 1) {
            int p = spf[(int)tmp];
            int cnt = 0;
            while (tmp % (size_t)p == 0) {
                tmp /= (size_t)p;
                ++cnt;
            }
            tau *= ((seq_t)cnt + 1);
        }

        sum += tau;
    }

    free(spf);
    return sum;
}
