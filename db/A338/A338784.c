/*
 * A338784  a(n) is the smallest number with exactly n divisors such that all
 *          its divisors end with the same digit (which is necessarily 1).
 * https://oeis.org/A338784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A338784:  smallest integer with n divisors, all divisors ending in 1.  */

#include <stdbool.h>
#include <stdint.h>

/* list of first 30 primes congruent to 1 (mod 10) */
static const uint64_t primes[30] =
{
    11, 31, 41, 61, 71, 101, 131, 151, 181, 191,
    211, 241, 251, 271, 281, 311, 331, 359, 379, 401,
    421, 431, 461, 491, 521, 541, 571, 601, 631, 661
};

static const unsigned int MAX_PRIMES = 30;

/* fast binary exponentiation for seq_t */
static seq_t pow_seq(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0)
    {
        if (exp & 1)
            result *= b;
        exp >>= 1;
        if (exp)
            b *= b;
    }
    return result;
}

/* simple primality test for small indices */
static bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

static seq_t best_val;

/* recursive search for minimal candidate */
static void search_rec(idx_t prime_idx, idx_t remaining, idx_t last_exp, seq_t cur)
{
    if (remaining == 1)
    {
        if (best_val == 0 || cur < best_val)
            best_val = cur;
        return;
    }

    if (prime_idx >= MAX_PRIMES) return;

    /* iterate over divisors d of 'remaining', d>=2, d<=last_exp+1 */
    for (idx_t d = 2; d <= remaining && d <= last_exp + 1; ++d)
    {
        if (remaining % d != 0) continue;
        idx_t e = d - 1;
        seq_t power = pow_seq(primes[prime_idx], e);

        /* simple pruning: if product already exceeds known best, skip */
        if (best_val && cur > best_val / power) continue;

        seq_t next = cur * power;
        if (best_val && next >= best_val) continue;

        search_rec(prime_idx + 1, remaining / d, e, next);
    }
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    if (is_prime_idx(n))
    {
        return pow_seq(11, n - 1);
    }

    best_val = 0;
    search_rec(0, n, n - 1, 1);
    return best_val;
}
