/*
 * A285770  Let q_1<q_2<... be primes not dividing 2n+1 such that 2n-q_i + 1
 *          is twice a prime. a(n) is smallest i such that 2n+2q_i+1 is prime
 *          or a(n)=0 if there is no such i.
 * https://oeis.org/A285770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest i such that q_i is a prime that does * not
 *            divide 2n+1, 2n–q_i+1 is twice a prime, and 2n+2q_i+1 is a *
 *            prime; 0 if no such i. Indexing starts at n = 1
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal runtime implementation – no external helper functions used. */
#include <stdbool.h>
#include <stdint.h>

/* Check whether n (n ≤ 40000 when called from a()) is a prime. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* sequence starts at n = 1 */

    idx_t m = 2 * n + 1;                   /* 2n+1 */
    idx_t two_n = 2 * n;                   /* 2n used many times */

    /* q must satisfy 2 <= q <= 2n-3 */
    idx_t limit_q = two_n >= 3 ? two_n - 3 : 0;

    uint64_t i = 0;                        /* number of qualifying q's seen */

    for (idx_t q = 2; q <= limit_q; ++q)
    {
        if (!is_prime_u64(q)) continue;              /* q must be prime */

        if (m % q == 0) continue;                    /* skip divisors of 2n+1 */

        idx_t diff = two_n >= q ? two_n - q + 1 : 0;  /* 2n - q + 1 */
        if (diff & 1) continue;                      /* must be even */

        idx_t p = diff >> 1;                         /* (2n - q + 1) / 2 */
        if (!is_prime_u64(p)) continue;               /* p must be prime */

        ++i;   /* this q is the (i)-th qualifying prime q_i */

        idx_t cand = two_n + (2 * q) + 1;            /* 2n + 2q + 1 */
        if (is_prime_u64(cand))
            return (seq_t)i;                         /* found desired i */
    }

    return (seq_t)0;                                 /* none found */
}
