/*
 * A347287  a(n) = Sum_{k = 1..m} 2^(e_k-1) where e_k =
 *          floor(log_p_k(p_(k-1)^e_(k-1))) such that e_k > 0.
 * https://oeis.org/A347287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence-based computation of A347287.
   e_1 = n.  For k ≥ 2,
      e_k = floor( e_{k‑1} * log(p_{k‑1}) / log(p_k) ),
   where p_k is the k‑th prime.
   The sequence is a(n) = Σ_k 2^(e_k‑1) until e_k = 0. */

#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t) */

/* Return the next prime after x (x ≥ 2). */
static uint64_t next_prime(uint64_t x)
{
    uint64_t q = x + 1;
    if (q <= 2ULL)          /* next prime after 2 is 2 itself */
        return 2ULL;
    if ((q & 1ULL) == 0ULL) q++;   /* make q odd */
    while (!is_prime(q))
        q += 2ULL;
    return q;
}

/* Main function as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n < 1 */

    /* First exponent e1 = n */
    seq_t sum = pow2(n - 1);

    /* Prepare for the first step (k = 2). */
    uint64_t prev_prime = 2ULL;          /* p_{k‑1}   */
    uint64_t curr_prime = 3ULL;          /* p_k       */
    idx_t   e_prev      = n;             /* e_{k‑1}   */

    while (1)
    {
        /* Compute e_k = floor(e_{k‑1} * log(p_{k‑1}) / log(p_k)) */
        long double ratio = (long double)e_prev *
                            logl((long double)prev_prime) /
                            logl((long double)curr_prime);
        idx_t e_cur = (idx_t)floorl(ratio);

        if (e_cur == 0)          /* no positive exponent left */
            break;

        /* Add 2^(e_k - 1) to the sum */
        sum += pow2(e_cur - 1);

        /* Advance to next step: shift primes and exponent */
        e_prev = e_cur;
        prev_prime = curr_prime;
        curr_prime = next_prime(curr_prime);
    }

    return sum;
}
