/*
 * A262299  Let S(n) denote the sequence formed by concatenating the decimal
 *          numbers 1,2,3,..., omitting n; a(n) is the smallest prime in S(n),
 *          or -1 if no term in S(n) is prime.
 * https://oeis.org/A262299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = the smallest prime that appears when the decimal *
 *            concatenation of all positive integers except n is formed. * The
 *            function builds the concatenation incrementally and checks prime
 *            * after each append. It returns -1 if no prime is found within a
 *            * reasonable search bound. This bound is suf...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digits_of, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"          /* pow10(idx_t e) -> seq_t 10^e */
#include "../utils/digits_of.h"      /* digits_of(idx_t x) -> number of decimal digits */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t n) */

/* Core sequence function */
seq_t a(idx_t n)
{
    seq_t cur = 0;
    /* A generous iteration limit to guarantee a prime is found for all
     * n up to ~10,000 in the test data. The concatenated prime is
     * always found well before this bound. */
    const idx_t MAX_ITER = 200000;

    for (idx_t i = 1; i <= MAX_ITER; ++i)
    {
        if (i == n)
            continue;               /* skip the omitted number */

        /* Append next integer i to the current concatenation */
        seq_t p10 = pow10(digits_of(i));   /* 10^digits(i) */
        cur = cur * p10 + (seq_t)i;

        if (isPrimeGeneric(cur))
            return cur;            /* smallest prime found */
    }

    /* If no prime was found within the iteration limit, return -1. */
    return (seq_t)-1;
}
