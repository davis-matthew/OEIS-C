/*
 * A387835  Irregular table read by rows: T(n,k) is the greatest number m such
 *          that m^k divides n, where k = 1..A051903(n).
 * https://oeis.org/A387835
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = concatenation of T(r,k) for r = 2,3,4,..., where T(r,k)
 *            is the largest m such that m^k divides r. For a given r, k runs
 *            from 1 to the maximum exponent e occurring in the prime
 *            factorisation of r. The sequence is concatenated starting from
 *            offset 2 (i.e., a(2) is the first term). This imple...
 * Range:     u8 n <= 424, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;               /* undefined below offset */

    /* position in the concatenated sequence (0‑based) */
    idx_t pos = n - 2;

    /* iterate over rows r = 2,3,... */
    for (idx_t r = 2; ; ++r) {
        /* factorisation of r: store primes and corresponding exponents */
        seq_t primes[64];
        int exps[64];
        size_t fcnt = 0;
        idx_t temp = r;

        /* trial division */
        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++cnt;
                }
                primes[fcnt] = (seq_t)p;
                exps[fcnt] = cnt;
                ++fcnt;
                if (temp == 1) break;
            }
        }
        if (temp > 1) {
            /* remaining prime factor */
            primes[fcnt] = (seq_t)temp;
            exps[fcnt] = 1;
            ++fcnt;
        }

        /* number of terms in this row: maximum exponent */
        int emax = 0;
        for (size_t i = 0; i < fcnt; ++i) {
            if (exps[i] > emax) emax = exps[i];
        }
        if (emax == 0) emax = 1;   /* safety for r==1 (not used) */

        if (pos < (idx_t)emax) {
            /* target found in this row */
            int k = (int)(pos + 1);          /* 1‑based k */
            seq_t result = (seq_t)1;
            for (size_t i = 0; i < fcnt; ++i) {
                int e = exps[i];
                int powExp = e / k;          /* floor(e/k) */
                if (powExp == 0) continue;
                seq_t mult = (seq_t)1;
                for (int j = 0; j < powExp; ++j) {
                    mult *= primes[i];
                }
                result *= mult;
            }
            return result;
        }

        /* skip this row and continue */
        pos -= (idx_t)emax;
    }
}
