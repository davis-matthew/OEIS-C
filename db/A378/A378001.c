/*
 * A378001  a(n) is the (n-1)-th positive integer that is digitally balanced
 *          in base n.
 * https://oeis.org/A378001
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (n-1)-th smallest integer whose base‑n digits * are a
 *            permutation of 0..n‑1 with leading digit non‑zero. * This is the
 *            same as: a(n) = A378000(n, n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned long long fact[21] = {
    1ULL,                        // 0!
    1ULL,                        // 1!
    2ULL,                        // 2!
    6ULL,                        // 3!
    24ULL,                       // 4!
    120ULL,                      // 5!
    720ULL,                      // 6!
    5040ULL,                     // 7!
    40320ULL,                    // 8!
    362880ULL,                   // 9!
    3628800ULL,                  // 10!
    39916800ULL,                 // 11!
    479001600ULL,                // 12!
    6227020800ULL,               // 13!
    87178291200ULL,              // 14!
    1307674368000ULL,            // 15!
    20922789888000ULL,           // 16!
    355687428096000ULL,          // 17!
    6402373705728000ULL,         // 18!
    121645100408832000ULL,       // 19!
    2432902008176640000ULL       // 20!
};

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0; /* undefined for the offset, but harmless */

    /* for n==2 we will follow the same general algorithm */
    idx_t rank = n - 1;              /* 1‑based rank of the desired number */
    idx_t k = n - 1;                 /* size of the remaining digits set */

    /* build the sorted set of remaining digits (0 and 2..n-1) */
    uint64_t *rem = (uint64_t *)malloc(k * sizeof(uint64_t));
    if (!rem)
        return (seq_t)0; /* fallback in case of allocation failure */

    size_t pos = 0;
    for (uint64_t d = 0; d < n; ++d)
        if (d != 1)
            rem[pos++] = d;          /* pos == k after loop */

    seq_t value = 0;
    /* leading digit is always 1 (because rank < (n-1)! for n>=3) */
    value = value * (seq_t)n + (seq_t)1;

    size_t len = k; /* current number of unused digits in rem */
    for (size_t i = 0; i < k; ++i)
    {
        size_t s = k - i - 1;            /* number of digits remaining after this one */
        size_t idx = 0;
        if (s < 10)
        {
            unsigned long long f = fact[s];
            idx = (size_t)((rank - 1ULL) / f);
            rank -= (idx * f);
        }
        /* idx is the position of the next digit in rem */
        uint64_t digit = rem[idx];

        /* compute the number in base n */
        value = value * (seq_t)n + (seq_t)digit;

        /* delete the used digit from rem (shift left) */
        for (size_t t = idx; t + 1 < len; ++t)
            rem[t] = rem[t + 1];
        --len;
    }

    free(rem);
    return value;
}
