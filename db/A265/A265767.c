/*
 * A265767  Numerators of upper primes-only best approximates (POBAs) to 5;
 *          see Comments.
 * https://oeis.org/A265767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A265767 – Numerators of upper primes‑only best approximates to 5
 *
 *  The sequence is defined inductively.  Start with the fraction
 *  p(1)/q(1) = 11/2.  For i>1, let p(i-1)/q(i-1) be the previously
 *  computed fraction.  Choose the least prime denominator q
 *  (q>q(i-1)) for which there exists a prime numerator p
 *  satisfying 5 < p/q < p(i-1)/q(i-1).  Among the primes p that
 *  satisfy this inequality pick the smallest such prime and set
 *  p(i)=p, q(i)=q.  Then a(i)=p(i).  The first terms are
 *  11, 37, 67, 97, 157, 307, 337, 367, ….
 *
 *  This implementation recomputes the sequence from scratch for each
 *  call.  The algorithm runs in O(n²) time but with n≈10 000 it
 *  finishes well under the 2‑second limit on typical machines.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- simple prime test (trial division) ---------- */
static inline bool is_prime_trial(uint64_t n)
{
    if (n < 2u) return false;
    if (n % 2u == 0u) return n == 2u;
    if (n % 3u == 0u) return n == 3u;
    /* check divisibility by 6k±1 up to sqrt(n) */
    for (uint64_t i = 5u; i * i <= n; i += 6u) {
        if (n % i == 0u || n % (i + 2u) == 0u)
            return false;
    }
    return true;
}

/* ---------- next prime strictly greater than n ---------- */
static inline uint64_t next_prime_gt(uint64_t n)
{
    if (n < 2u) return 2u;
    uint64_t cand = n + 1u;
    /* make cand odd (except for 2) */
    if (cand % 2u == 0u) cand = (cand == 2u) ? 2u : cand + 1u;
    while (!is_prime_trial(cand))
        cand += 2u;
    return cand;
}

/* ---------- compute the n-th numerator ---------- */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;          /* only defined for n≥1 */

    /* In the loop variables use 64‑bit types – everything fits into
       at most 64 bits for the tested range.  Cross‑products use
       unsigned __int128 to avoid overflow. */
    uint64_t prev_p = 11u;   /* numerator of previous fraction   */
    uint64_t prev_q = 2u;    /* denominator of previous fraction */

    if (n == 1u) return (seq_t)prev_p;

    for (uint64_t i = 2u; i <= n; ++i) {
        /* Find the smallest prime denominator q>prev_q such that
           there exists a prime p with 5 < p/q < prev_p/prev_q. */
        uint64_t q = next_prime_gt(prev_q);
        /* We search such q incrementally. */
        while (true) {
            /* minimal prime numerator > 5*q */
            uint64_t p = next_prime_gt(5u * q);

            /* Check whether p/q < prev_p/prev_q.
               Use a cross‑product with 128‑bit arithmetic for safety. */
            __uint128_t lhs = (__uint128_t)p * prev_q;
            __uint128_t rhs = (__uint128_t)prev_p * q;
            if (lhs < rhs) {
                /* Found the next fraction. */
                prev_p = p;
                prev_q = q;
                break;
            }
            /* Otherwise try the next prime denominator. */
            q = next_prime_gt(q);
        }
    }
    return (seq_t)prev_p;
}
