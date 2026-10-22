/*
 * A346608  Indices k such that A047994(k) != A344005(k).
 * https://oeis.org/A346608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 8020, u32 n <= 8020, u64 n <= 8020,
 *            u128 n <= 8020, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A346608: indices k (k ≥ 1) such that A047994(k) ≠ A344005(k).
   It turns out that these are exactly the composite numbers
   except 18, numbers of the form 2·p^i (p prime, i ≥ 1),
   and prime‑power numbers p^i (p prime, i ≥ 2).  */
#include <stdint.h>
#include <stdbool.h>

/* --- Helper ----------------------------------------------------------- */

/* Simple trial‑division primality test.  Good for numbers up to ~10⁶
   and far larger for the sequence up to at least n = 10 000. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)            return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0)     return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)   return false;
    return true;
}

/* Smallest prime factor of a composite number n (> 1).
   If n is prime the function returns n itself.             */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if ((n & 1) == 0)     return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)   return d;
    return n;             /* n is prime */
}

/* Tests whether m is a prime power p^i with i ≥ 2.  The function
   returns false for a prime (i = 1) or for m ≤ 1.              */
static bool is_prime_power(uint64_t m)
{
    if (m <= 2)            return false;
    uint64_t p = smallest_prime_factor(m);
    if (p == m)            return false;   /* m is prime */
    while (m % p == 0)     m /= p;
    return m == 1;
}

/* --- Public function ------------------------------------------------- */

/* Return the n‑th element of A346608 (indices 1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 4;                /* first composite > 3 */

    while (1) {
        /* Skip non‑composites. */
        if (k <= 3 || is_prime_uint64(k)) {
            ++k;
            continue;
        }

        /* Exclude the special number 18 (already handled by the rule below,
           but kept for explicitness). */
        if (k == 18) {
            ++k;
            continue;
        }

        /* Exclude numbers of the form 2·p^i (p prime, i ≥ 1). */
        if ((k & 1) == 0) {
            uint64_t m = k >> 1;    /* m = k/2 */
            if (is_prime_uint64(m) || is_prime_power(m)) {
                ++k;
                continue;
            }
        }

        /* Exclude pure prime powers (p^i, i ≥ 2). */
        if (is_prime_power(k)) {
            ++k;
            continue;
        }

        /* k satisfies all conditions → it is the next term. */
        ++count;
        if (count == n)
            return (seq_t)k;
        ++k;
    }
}
