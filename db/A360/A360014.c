/*
 * A360014  Numbers whose exponent of 2 in their canonical prime factorization
 *          is equal to the maximum of the other exponents.
 * https://oeis.org/A360014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360014: A360014(n) is the n‑th positive integer m for which
 * the exponent of 2 in the prime factorization of m equals the maximum
 * of the exponents of the odd primes dividing m.  The sequence contains
 * 1, 6, 10, 14, 22, 26, 30, 34, 36, ...
 *
 * The function below searches the positive integers in order,
 * testing the condition above for each candidate.  The test uses only
 * integer operations on the type seq_t (no floating point or external
 * libraries).  For the modest values of n required in the tests (<≈10000)
 * this O(n) search completes well within the time limit. */

#include <stdbool.h>
#include <stdint.h>

/* Return true if x satisfies the defining property of A360014. */
static bool in_sequence(seq_t x)
{
    if (x == (seq_t)1)          /* the number 1 is always in the sequence */
        return true;

    /* Separate the 2‑part and count its exponent. */
    unsigned v2 = 0;
    seq_t ux = x;
    while ((ux & (seq_t)1) == (seq_t)0) {
        v2++;
        ux >>= 1;
    }

    /* If the odd part is 1 then m has no odd prime factors. */
    if (ux == (seq_t)1)
        return false;           /* we need v2 == 0, which cannot happen here */

    /* Factor the odd part, keeping the maximum exponent seen. */
    unsigned maxexp = 0;
    seq_t temp = ux;
    for (seq_t p = (seq_t)3; p * p <= temp; p += 2) {
        if (temp % p == (seq_t)0) {
            unsigned exp = 0;
            while (temp % p == (seq_t)0) {
                temp /= p;
                exp++;
            }
            if (exp > maxexp)
                maxexp = exp;
        }
    }
    if (temp > (seq_t)1)           /* remaining prime factor */
        maxexp = 1 > maxexp ? 1 : maxexp;

    return v2 == maxexp;
}

/* Determine the n-th element of A360014. */
seq_t a(idx_t n)
{
    if (n == 1)          /* the first term is 1 */
        return (seq_t)1;

    idx_t count = 1;     /* we have already found the term 1 */
    seq_t m = (seq_t)2;  /* start testing from 2 */

    while (count < n) {
        if (in_sequence(m)) {
            count++;
            if (count == n)
                return m;
        }
        m++;                  /* next integer */
    }

    /* Should never reach here. */
    return (seq_t)0;
}
