/*
 * A248784  Number of squares that divide 1!*2!*3!*...*n!.
 * https://oeis.org/A248784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} (⌊e_p/2⌋ + 1) where * e_p = Σ_{t≥1} (⌊n /
 *            p^t⌋*(n+1) - p^t*⌊n/p^t⌋*(⌊n/p^t⌋+1)/2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached prime list up to the largest n requested. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */
static idx_t    prime_limit = 1;      /* largest number that has been sieved */

/* Extend the prime list so that all primes ≤ limit are present. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;   /* already up to date */

    for (idx_t cand = prime_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;          /* skip 0 and 1 */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > (uint64_t)cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                    (prime_count + 1) * sizeof(uint64_t));
            if (!tmp) exit(1);            /* allocation failure */
            prime_list = tmp;
            prime_list[prime_count++] = cand;
        }
    }
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* offset starts at 1 – value for n<1 does not matter */

    ensure_primes(n);

    seq_t ans = 1;

    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > (uint64_t)n) break;

        seq_t e = 0;
        idx_t d = p;                       /* current power of the prime (p^t) */
        while (d <= n) {
            idx_t m = n / d;                /* ⌊n/d⌋ */
            seq_t term1 = ((seq_t)d * (seq_t)m * (seq_t)(m - 1)) / 2;
            seq_t term2 = (seq_t)m * (seq_t)(n - m * d + 1);
            e += term1 + term2;

            if (d > n / p) break;           /* next power would exceed n */
            d *= p;
        }

        seq_t factor = (e / 2) + 1;          /* ⌊e/2⌋ + 1 */
        ans *= factor;
    }

    return ans;
}
