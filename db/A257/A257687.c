/*
 * A257687  Discard the most significant digit from factorial base
 *          representation of n, then convert back to decimal: a(n) = n -
 *          A257686(n).
 * https://oeis.org/A257687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number obtained by discarding the most significant digit
 *            from n's factorial base representation and converting the
 *            remainder back to decimal
 * Range:     u8 n <= 975, u16 n <= 8260, u32 n <= 8260, u64 n <= 8260,
 *            u128 n <= 8260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A257687. */
#include <stdint.h>
#include <stdbool.h>

/* idx_t and seq_t are defined by the build system */

/* Pre‑computed factorials up to 20! (fits in 64‑bit, the largest needed
   for n <= UINT64_MAX). */
static const idx_t FACT[21] = {
    1ULL,          /* 0! (unused) */
    1ULL,          /* 1! */
    2ULL,          /* 2! */
    6ULL,          /* 3! */
    24ULL,         /* 4! */
    120ULL,        /* 5! */
    720ULL,        /* 6! */
    5040ULL,       /* 7! */
    40320ULL,      /* 8! */
    362880ULL,     /* 9! */
    3628800ULL,    /*10!*/
    39916800ULL,   /*11!*/
    479001600ULL,  /*12!*/
    6227020800ULL, /*13!*/
    87178291200ULL,/*14!*/
    1307674368000ULL, /*15!*/
    20922789888000ULL,/*16!*/
    355687428096000ULL,/*17!*/
    6402373705728000ULL,/*18!*/
    121645100408832000ULL,/*19!*/
    2432902008176640000ULL /*20!*/
};

seq_t a(idx_t n)
{
    /* Special case: n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Determine the largest k such that k! <= n. */
    int k = 1;
    while (k < 20 && FACT[k + 1] <= n)
        ++k;

    /* Compute digits and the truncated value. */
    idx_t rem = n;
    idx_t truncated = 0;

    for (int i = k; i >= 1; --i) {
        idx_t digit = rem / FACT[i];
        rem -= digit * FACT[i];
        if (i < k)          /* discard the most significant digit (i == k) */
            truncated += digit * FACT[i];
    }

    return (seq_t)truncated;
}
