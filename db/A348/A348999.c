/*
 * A348999  a(n) = A348929(A276086(n)), where A348929(n) = gcd(n, A003959(n)),
 *          A003959 is multiplicative with a(p^e) = (p+1)^e, and A276086 gives
 *          the prime product form of primorial base expansion of n.
 * https://oeis.org/A348999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = gcd(A276086(n), A003959(A276086(n)))
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 598, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   get_prime, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where A003959 is multiplicative with a(p^e) = (p+1)^e,
    and A276086(n) is the product ∏ p_i^d_i with the primorial‑base
    digits d_i of n. */

#include <stdint.h>
#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"
#include "../utils/get_prime.h"

#define MAX_PRIMES 32   /* enough for n ≤ 10000 */

seq_t a(idx_t n)
{
    /* A276086(0) is defined to be 1; gcd(1,1)=1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build primorials primorial[i] = ∏_{k=1}^{i} p_k, with primorial[0]=1.
       Store them until the next would exceed n. */
    seq_t primorial[MAX_PRIMES];
    primorial[0] = (seq_t)1;
    size_t count = 0;          /* last index stored in primorial[] */
    seq_t cur = (seq_t)1;

    while (count + 1 < MAX_PRIMES) {
        idx_t prime = get_prime((idx_t)(count + 1));   /* get kth prime, k=1⇒2 */
        seq_t next = cur * (seq_t)prime;
        if (next > (seq_t)n) {
            break;
        }
        primorial[++count] = next;
        cur = next;
    }

    /* Highest primorial index that is ≤ n. */
    size_t max_i = count;

    /* Extract primorial‑base digits d_i.  Each digit satisfies
       0 ≤ d_i ≤ p_{i+1}-1, where p_{i+1} = get_prime(i+2). */
    idx_t digits[MAX_PRIMES];
    for (size_t i = 0; i <= max_i; ++i) {
        digits[i] = 0;
    }

    idx_t rem = n;
    for (int i = (int)max_i; i >= 0; --i) {
        seq_t weight = primorial[(size_t)i];
        idx_t digit = rem / (idx_t)weight;
        idx_t max_digit = get_prime((idx_t)(i + 2)) - 1;
        if (digit > max_digit) {
            digit = max_digit;
        }
        digits[(size_t)i] = digit;
        rem -= digit * (idx_t)weight;
        if (i == 0) break;   /* avoid unsigned underflow */
    }

    /* Compute A276086(n) and A003959(A276086(n)). */
    seq_t m  = (seq_t)1;   /* product of p_i^d_i */
    seq_t X  = (seq_t)1;   /* product of (p_i+1)^d_i */

    for (size_t i = 0; i <= max_i; ++i) {
        idx_t exp = digits[i];
        if (exp == 0) continue;

        seq_t prime  = (seq_t)get_prime((idx_t)(i + 1));     /* p_i */
        seq_t pplus  = prime + 1;                           /* p_i + 1 */

        seq_t pow_m = seq_pow(prime, exp);
        seq_t pow_X = seq_pow(pplus, exp);

        m *= pow_m;
        X *= pow_X;
    }

    return seq_gcd(m, X);
}
