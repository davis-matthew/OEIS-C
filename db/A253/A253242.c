/*
 * A253242  Least k>=0 such that n^(2^k)+1 is prime (for even n), or
 *          (n^(2^k)+1)/2 is prime (for odd n); -1 if no such k exists.
 * https://oeis.org/A253242
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, more, hard
 * Formula:   a(n) = least k>=0 such that n^(2^k)+1 is prime (if n even) or
 *            (n^(2^k)+1)/2 is prime (if n odd); a(n) = -1 if n is a perfect
 *            power with an odd exponent or if no such k exists
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 22, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   big_pow, factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* integer GCD for small exponents */
static int int_gcd(int a, int b)
{
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* true if n = p^e with e odd and e>1  */
static bool is_perfect_power_odd_exponent(uint64_t n)
{
    if (n < 2) return false;
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    if (cnt == 0) return false;              /* prime */
    if (cnt == 1) {                            /* single prime factor */
        return (exps[0] > 1) && ((exps[0] & 1) == 1);
    }
    int g = (int)exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        g = int_gcd(g, (int)exps[i]);
        if (g == 1) break;
    }
    return (g > 1) && ((g & 1) == 1);
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)-1;               /* below offset */
    if (is_perfect_power_odd_exponent((uint64_t)n)) return (seq_t)-1;

    /* Search up to a reasonable bound; for n ≤ 10000 this is sufficient. */
    const unsigned MAX_K = 12;                 /* 2^12 = 4096 */
    for (unsigned k = 0; k <= MAX_K; ++k) {
        idx_t exp = 1ULL << k;                 /* 2^k */
        seq_t val = big_pow((seq_t)n, (seq_t)exp);
        val = val + (seq_t)1;
        if ((n & 1ULL) != 0) {                 /* odd n: divide by 2 */
            val = val / (seq_t)2;
        }
        if (is_prime_big(val)) return (seq_t)k;
    }
    return (seq_t)-1;                          /* no prime found within bound */
}
