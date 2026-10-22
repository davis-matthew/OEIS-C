/*
 * A301652  Triangle read by rows: row n gives the digits of n in factorial
 *          base in reversed order.
 * https://oeis.org/A301652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 8656, u16 n <= 8656, u32 n <= 8656, u64 n <= 8656,
 *            u128 n <= 8656, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes sequence A301652.
 *
 * The sequence is obtained by flattening the triangle
 * T(n,k) = floor(n/k!) mod (k+1)  (k ≥ 1) which gives the digits of n
 * in factorial base, printed in reversed order (least significant first).
 * Row 0 consists solely of the digit 0.
 *
 * To find a(n) we locate the row r such that
 *   Σ_{i<r} len(i) ≤ n < Σ_{i≤r} len(i),
 *   where len(i) = 1 for i=0 and len(i) = max { k | k! ≤ i } for i>0.
 * Then a(n) = T(r, k) where k = (n – Σ_{i<r} len(i)) + 1.
 */

#include <stddef.h>
#include <stdint.h>

static const uint64_t FACT[21] = {
    1ULL,                       /* 0!   */
    1ULL,                       /* 1!   */
    2ULL,                       /* 2!   */
    6ULL,                       /* 3!   */
    24ULL,                      /* 4!   */
    120ULL,                     /* 5!   */
    720ULL,                     /* 6!   */
    5040ULL,                    /* 7!   */
    40320ULL,                   /* 8!   */
    362880ULL,                  /* 9!   */
    3628800ULL,                 /* 10!  */
    39916800ULL,                /* 11!  */
    479001600ULL,               /* 12!  */
    6227020800ULL,              /* 13!  */
    87178291200ULL,             /* 14!  */
    1307674368000ULL,           /* 15!  */
    20922789888000ULL,          /* 16!  */
    355687428096000ULL,         /* 17!  */
    6402373705728000ULL,        /* 18!  */
    121645100408832000ULL,      /* 19!  */
    2432902008176640000ULL      /* 20!  */
};

/* digit of n in factorial base at position k (k ≥ 1) */
static inline seq_t digit_factorial_base(uint64_t n, uint64_t k)
{
    uint64_t f = FACT[k];
    uint64_t d = (n / f) % (k + 1);
    return (seq_t)d;
}

seq_t a(idx_t n)
{
    /* Row 0 consists of the single digit 0. */
    if (n == 0)
        return (seq_t)0;

    /* Find the row containing the n-th flattened element. */
    idx_t row = 1;            /* start after row 0 */
    idx_t cumulative = 1;     /* elements before current row */

    while (1)
    {
        /* Determine length of this row (number of digits). */
        uint64_t r = (uint64_t)row;
        uint64_t k = 1;
        while (k + 1 < 21 && FACT[k + 1] <= r)
            ++k;
        size_t len = (size_t)k;   /* row length */

        if (cumulative + len > n)   /* target lies in this row */

        {
            size_t offset = (size_t)(n - cumulative);   /* 0-based offset in row */
            uint64_t digit_k = offset + 1;               /* k = offset+1 (since digits start at k=1) */
            return digit_factorial_base(r, digit_k);
        }

        cumulative += len;
        ++row;
    }
}
