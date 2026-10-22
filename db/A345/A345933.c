/*
 * A345933  a(n) = n / gcd(n, A002034(n)), where A002034(n) gives the smallest
 *          positive integer k such that n divides k!.
 * https://oeis.org/A345933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, A002034(n))
 * Range:     u8 n <= 766, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002034(n) is the least k such that n | k! */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static seq_t min_k_for_prime_power(seq_t p, seq_t e)
{
    /* minimal k with Σ floor(k/p^i) >= e  */
    seq_t low = 1, high = e * p;  /* high is always sufficient */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t sum = 0;
        for (seq_t t = mid / p; t > 0; t /= p)
            sum += t;
        if (sum >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t max_k = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        seq_t kp = min_k_for_prime_power(p, e);
        if (kp > max_k)
            max_k = kp;
    }

    seq_t g = gcd((seq_t)n, max_k);
    return ((seq_t)n) / g;
}
