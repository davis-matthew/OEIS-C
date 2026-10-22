/*
 * A378370  Distance between n and the least prime power >= n, allowing 1.
 * https://oeis.org/A378370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000015(n) – n → distance to the least prime power ≥ n
 *            (prime powers include 1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether x is a prime power (1 counts as a prime power) */
static bool is_prime_power(seq_t x)
{
    if (x <= (seq_t)1)                          /* 1 is a prime power    */
        return true;

    idx_t rem   = (idx_t)x;                     /* value to factorise     */
    seq_t distinct = 0;                         /* count of distinct primes */

    /* Trial division; continue while p*p <= rem */
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= rem; ++p) {
        if (rem % p == 0) {
            ++distinct;
            if (distinct > 1)                   /* more than one prime factor */
                return false;

            /* strip all powers of p */
            while (rem % p == 0)
                rem /= p;
        }
    }

    /* Remaining rem > 1 is a prime factor */
    if (rem > 1)
        ++distinct;

    return distinct == 1;
}

/* OEIS sequence A378370 */
seq_t a(idx_t n)
{
    idx_t cur = (idx_t)n;                       /* current candidate      */

    while (!is_prime_power((seq_t)cur))
        ++cur;                                 /* seek next prime power  */

    return (seq_t)cur - (seq_t)n;               /* distance to it         */
}
