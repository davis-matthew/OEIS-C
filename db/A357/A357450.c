/*
 * A357450  a(n) is the smallest integer having exactly n odd square divisors
 *          (A298735).
 * https://oeis.org/A357450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A038547(n)^2, where A038547(n) is the smallest odd
 *            integer having exactly n odd square divisors. The algorithm
 *            searches for the minimal odd integer product p_i^(2*e_i) with
 *            product_i (e_i+1)=n by a depth‑first recursion that keeps
 *            exponents non‑increasing
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

static const uint64_t odd_primes[] = {
    3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL,
    37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL,
    79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
    127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL,
    167ULL, 173ULL, 179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL
};
#define ODD_PRIME_COUNT (sizeof(odd_primes) / sizeof(odd_primes[0]))

/* Depth‑first search for the minimal odd integer S with
   product_i (e_i+1) = n.  cur holds the current product, best
   tracks the smallest product found so far. */
static void dfs(idx_t rem, idx_t max_exp, size_t pi, seq_t cur,
                seq_t *best, const uint64_t *primes, size_t primes_len)
{
    if (rem == 1) {                 /* all factors accounted for */
        if (*best == 0 || cur < *best) *best = cur;
        return;
    }
    if (pi == primes_len) return;   /* ran out of primes */

    seq_t p_pow = (seq_t)1;
    uint64_t prime = primes[pi];

    for (idx_t e = 1; e <= max_exp; ++e) {
        p_pow = mul_small(p_pow, (unsigned int)prime);
        idx_t factor = e + 1;
        if (rem % factor != 0) continue;   /* e+1 must divide remaining count */

        seq_t next = seq_mul(cur, p_pow);
        if (*best && next >= *best) break; /* further e only increase next */

        dfs(rem / factor, e, pi + 1, next, best, primes, primes_len);
    }
}

/* Compute the minimal odd integer S with exactly n odd square divisors. */
static seq_t minimal_odd_integer(idx_t n)
{
    if (n == 0) return 0;
    seq_t best = 0;
    dfs(n, n, 0, (seq_t)1, &best, odd_primes, ODD_PRIME_COUNT);
    return best;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */
    seq_t s = minimal_odd_integer(n);
    return seq_mul(s, s);               /* a(n) = s^2 */
}
