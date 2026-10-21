/*
 * A249738  a(n) = 0 if n is 1 or a prime, otherwise, when n = A020639(n) *
 *          A032742(n), a(n) = the largest k < A032742(n) such that either k =
 *          1 or A020639(k) >= A020639(n), where A020639(n) and A032742(n) are
 *          the smallest prime and the largest proper divisor dividing n.
 * https://oeis.org/A249738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * A249738 is defined by * a(1) = 0, a(prime) = 0. * For
 *            composite n, let p = smallest prime factor of n, * d = n / p.
 *            Then a(n) is the largest k < d such that * k = 1 or the smallest
 *            prime factor of k is >= p. * Return that k (as seq_t)
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Trivial smallest‑prime‑factor routine for n <= about 1e10.
 * Uses the 6k ± 1 wheel. */
static inline uint64_t smallest_prime_factor(uint64_t n)
{
    if (n < 2) return n;
    if ((n & 1ULL) == 0ULL) return 2ULL;
    if (n % 3ULL == 0ULL)   return 3ULL;
    uint64_t i   = 5ULL;
    uint64_t inc = 2ULL;
    /* i * i may overflow, use division */
    for (; i <= n / i; i += inc, inc = 6ULL - inc)
        if (n % i == 0ULL) return i;
    return n; /* n is prime */
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn <= 1ULL) return 0;            /* a(1) = 0 */

    uint64_t p = smallest_prime_factor(nn);
    if (p == nn) return 0;              /* prime n */

    uint64_t d = nn / p;                 /* largest proper divisor */

    for (uint64_t k = d - 1ULL; k > 0ULL; --k)
    {
        if (k == 1ULL) return (seq_t)1;  /* k=1 always acceptable */
        uint64_t spf_k = smallest_prime_factor(k);
        if (spf_k >= p) return (seq_t)k;
    }
    return (seq_t)1; /* unreachable */
}
