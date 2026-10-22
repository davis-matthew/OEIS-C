/*
 * A398264  Numbers divisible by the square but not the cube of their least
 *          prime factor.
 * https://oeis.org/A398264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the integers x that are divisible by the
   square of their least prime factor lpf(x) but not by its cube.
   We compute the terms by brute‑force search: for each candidate
   integer we find lpf(x) by trial division and test the required
   condition.  No memoisation or external utilities are used. */

#include <stdint.h>
#include <stddef.h>

/* Return the least prime factor of x (x ≥ 2).  We use a simple
   trial division; the result is the prime itself if x is prime. */
static seq_t smallest_prime_factor(seq_t x)
{
    for (seq_t d = 2; d * d <= x; d++) {
        if (x % d == 0) {
            return d;
        }
    }
    return x;   /* x is prime */
}

/* seq_t a(idx_t n)
   Find the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many terms found so far   */
    seq_t x = 2;              /* current candidate integer     */

    while (found < n) {
        seq_t p  = smallest_prime_factor(x);
        seq_t sp2 = p * p;            /* p²  */
        seq_t sp3 = sp2 * p;          /* p³  */

        if (x % sp2 == 0 && x % sp3 != 0) {
            found++;
            if (found == n) {
                return x;
            }
        }
        x++; /* move to next integer */
    }
    /* unreachable – keeps compiler happy */
    return 0;
}
