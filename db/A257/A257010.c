/*
 * A257010  Number of sequences of positive integers with length 3 and
 *          alternant equal to n.
 * https://oeis.org/A257010
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{b=1..n-1} ( Dbm(b,b^2+nb+1) - 2 ) where Dbm(b,m) =
 *            number of positive divisors of m that are ≡ 1 (mod b)
 * Range:     u8 n <= 13, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _count_excluded_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_count_excluded_divs.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;         /* values below offset are undefined */

    seq_t result = 0;

    for (idx_t b = 1; b < n; ++b) {
        /* compute m = b^2 + n*b + 1 up to 64-bit */
        uint64_t m = (uint64_t)b * (uint64_t)b + (uint64_t)n * (uint64_t)b + 1ULL;

        /* factor m */
        uint64_t temp = m;
        uint64_t primes[12];
        int      exps[12];
        int      pc = 0;

        if (temp % 2 == 0) {
            int e = 0;
            while (temp % 2 == 0) {
                temp /= 2;
                ++e;
            }
            primes[pc] = 2;
            exps[pc]   = e;
            ++pc;
        }
        for (uint64_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                int e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                primes[pc] = p;
                exps[pc]   = e;
                ++pc;
            }
        }
        if (temp > 1) {
            primes[pc] = temp;
            exps[pc]   = 1;
            ++pc;
        }

        seq_t cnt = 0;

        if (b == 1) {
            /* all divisors of m are counted */
            seq_t all = 1;
            for (int i = 0; i < pc; ++i)
                all *= (seq_t)(exps[i] + 1);
            cnt = all;
        } else {
            /* count divisors ≡ 1 mod b */
            _count_excluded_divs(primes, exps, pc, 0, 1, b, &cnt);
        }

        /* subtract 1 and m; ensure cnt >= 2 */
        if (cnt >= 2)
            result += (cnt > 2 ? (cnt - 2) : 0);
    }

    return result;
}
