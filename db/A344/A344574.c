/*
 * A344574  Number of ordered pairs (i,j) with 0 < i < j < n such that
 *          gcd(i,j,n) > 1.
 * https://oeis.org/A344574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Σ_{∅≠S⊆P(n)} (-1)^{|S|+1} C( ⌊(n‑1)/∏S⌋ , 2 ) where P(n)
 *            is the set of distinct prime divisors of n
 * Range:     u8 n <= 41, u16 n <= 629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* For n ≤ 2 there are no admissible pairs. */
    if (n <= 2) return 0;

    /* Collect the distinct prime factors of n.  The number of
       distinct primes ≤ 16 for n ≤ 10 000, so a fixed array is fine. */
    uint64_t primes[16];
    size_t k = 0;
    idx_t m = n;          /* n is uint64_t in idx_t, safe to copy */

    if ((m & 1ULL) == 0) {
        primes[k++] = 2ULL;
        while ((m & 1ULL) == 0) m >>= 1;
    }
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            primes[k++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) primes[k++] = m;

    seq_t result = 0;

    /* Enumerate all non‑empty subsets of the prime factors. */
    size_t subsets = 1U << k;
    for (size_t mask = 1; mask < subsets; ++mask) {
        /* product of primes in this subset */
        idx_t d = 1;
        for (size_t i = 0; i < k; ++i)
            if (mask & (1ULL << i))
                d *= primes[i];

        /* number of integers < n that are multiples of d */
        idx_t cnt = (n - 1U) / d;
        if (cnt < 2) continue;          /* C(cnt,2) = 0 */

        seq_t pairs = (seq_t)cnt * ((seq_t)cnt - 1) / 2;
        /* inclusion‑exclusion sign */
        if (__builtin_popcountll(mask) & 1U)
            result += pairs;
        else
            result -= pairs;
    }

    return result;
}
