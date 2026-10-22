/*
 * A399734  The sum of the unitary divisors d of n such that n/d is a cubefull
 *          number (A036966).
 * https://oeis.org/A399734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over all prime powers p^e || n of f(p,e), where
 *            f(p,e) = p^e for e ≤ 2 p^e + 1 for e ≥ 3 (i.e. a is
 *            multiplicative with a(p^e) = p^e for e ≤ 2, a(p^e)=p^e+1 for
 *            e≥3)
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static void build_small_primes(unsigned **primes_out, size_t *cnt_out)
{
    /* Build a simple list of primes up to LIMIT using the Sieve of Eratosthenes.
       The upper limit is chosen to cover numbers up to at least 10^5, which is
       more than sufficient for n up to 10 000. */
    const unsigned LIMIT = 100000;
    bool *mark = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!mark) exit(1);
    for (unsigned i = 2; i <= LIMIT; ++i) mark[i] = true;

    size_t primes_cnt = 0;
    for (unsigned i = 2; i * i <= LIMIT; ++i) {
        if (mark[i]) {
            for (unsigned j = i * i; j <= LIMIT; j += i) mark[j] = false;
        }
    }
    /* Count primes */
    for (unsigned i = 2; i <= LIMIT; ++i) if (mark[i]) ++primes_cnt;

    unsigned *primes = (unsigned *)malloc(primes_cnt * sizeof(unsigned));
    if (!primes) exit(1);
    for (unsigned i = 2, idx = 0; i <= LIMIT; ++i) {
        if (mark[i]) primes[idx++] = i;
    }
    free(mark);
    *primes_out = primes;
    *cnt_out = primes_cnt;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static unsigned *primes = NULL;
    static size_t primes_cnt = 0;

    if (!initialized) {
        build_small_primes(&primes, &primes_cnt);
        initialized = true;
    }

    if (n == 0) return 0;          /* undefined for 0, but guard */
    idx_t temp = n;
    seq_t result = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        unsigned p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t p_pow = 1;
            for (int k = 0; k < exp; ++k) p_pow *= (seq_t)p;
            if (exp <= 2)
                result *= p_pow;
            else
                result *= (p_pow + 1);
        }
    }
    /* Remaining factor, if any, is prime (exponent = 1) */
    if (temp > 1) {
        result *= (seq_t)temp;     /* exp = 1 ≤ 2 */
    }
    return result;
}
