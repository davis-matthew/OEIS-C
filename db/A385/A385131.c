/*
 * A385131  The sum of divisors of n whose maximum exponent in their prime
 *          factorization is odd.
 * https://oeis.org/A385131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * (A051903(d) mod 2)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of n whose maximum prime‑exponent in
   the divisor is odd by enumerating all divisors of n. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Factorise n by trial division. */
    uint64_t temp = (uint64_t)n;
    /* maximum distinct prime factors for 64‑bit integers is <64 */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    int     exps[MAX_FACTORS];
    size_t  cnt = 0, i, j;

    if (temp == 1) return (seq_t)0;   /* n = 1 yields no odd‑max divisor */

    /* factor 2 */
    if (temp % 2ULL == 0) {
        int e = 0;
        while ((temp % 2ULL) == 0) {
            temp /= 2ULL;
            e++;
        }
        primes[cnt] = 2ULL;
        exps[cnt]   = e;
        cnt++;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            cnt++;
        }
    }
    /* remaining prime > sqrt(n) */
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        cnt++;
    }

    /* Enumerate all divisors with their maximal exponent. */
    size_t  fac_cnt = cnt;
    size_t* idx     = (size_t*)calloc(fac_cnt, sizeof(size_t));
    seq_t  *pow_arr = (seq_t*)malloc(fac_cnt * sizeof(seq_t));
    for (i = 0; i < fac_cnt; ++i) pow_arr[i] = (seq_t)1;

    seq_t sum = (seq_t)0;

    while (1) {
        /* compute current divisor and its maximum exponent */
        seq_t d = (seq_t)1;
        int   maxExp = 0;
        for (i = 0; i < fac_cnt; ++i) {
            d *= pow_arr[i];
            if (idx[i] > (size_t)maxExp) maxExp = (int)idx[i];
        }
        if (maxExp & 1) sum += d;

        /* increment indices like a mixed‑radix counter */
        for (i = 0; i < fac_cnt; ++i) {
            if (idx[i] < (size_t)exps[i]) {
                idx[i]++;
                pow_arr[i] *= (seq_t)primes[i];
                break;
            } else {
                idx[i] = 0;
                pow_arr[i] = (seq_t)1;
            }
        }
        if (i == fac_cnt) break;  /* finished all combinations */
    }

    free(idx);
    free(pow_arr);
    return sum;
}
