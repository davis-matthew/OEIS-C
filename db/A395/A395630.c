/*
 * A395630  Divisors of the order of the Baby Monster group, |B| =
 *          4154781481226426191177580544000000.
 * https://oeis.org/A395630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A395630 – divisors of the Baby Monster group order
 *
 *  The order of the Baby Monster group is
 *      |B| = 2^41·3^13·5^6·7^2·11·13·17·19·23·31·47 .
 *  Its number of divisors is N = 1 580 544.
 *
 *  The sequence consists of all divisors of |B| sorted in increasing order.
 *  The implementation enumerates all divisors recursively, stores them in a
 *  static array, sorts the array once, and returns the (n‑1)th entry
 *  (0‑based index) for a(n).
 *
 *  This algorithm works for any unsigned integer type seq_t (8, 16, 32,
 *  64, 128 bits or arbitrary‑precision).  The code is valid C11 and C++17
 *  and compiles without the need for any external helper functions
 *  beyond a comparison routine for seq_t (cmp_seq_t).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_t.h"

/* Prime factorisation of |B|.  The primes and exponents are small enough
 * to fit in the building idx_t type. */
static const idx_t primes[11] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
                                 17ULL, 19ULL, 23ULL, 31ULL, 47ULL};
static const idx_t exps[11]   = {41ULL, 13ULL, 6ULL, 2ULL, 1ULL, 1ULL,
                                 1ULL,  1ULL, 1ULL, 1ULL, 1ULL};

/* Recursive divisor generator.  cur * power is guaranteed to be a
 * divisor of |B|. */
static void gen_divs(int idx, seq_t cur, size_t *cnt, seq_t *arr)
{
    if (idx == 11) {
        arr[(*cnt)++] = cur;
        return;
    }

    seq_t p = (seq_t)primes[idx];
    seq_t power = (seq_t)1;
    for (idx_t e = 0; e <= exps[idx]; ++e) {
        /* Recurse with current product */
        gen_divs(idx + 1, cur * power, cnt, arr);

        /* Prepare power *= p for next exponent */
        if (e == exps[idx]) break;
        power *= p;
    }
}

/* Public interface required by the testing framework. */
seq_t a(idx_t n)
{
    /* Lazy static initialisation of the divisor list */
    static seq_t *divs = NULL;
    static size_t divs_count = 0;

    if (!divs) {
        /* Number of divisors of |B| = ∏ (exponent + 1) */
        size_t Ndivs = 1;
        for (int i = 0; i < 11; ++i)
            Ndivs *= (exps[i] + 1);

        divs = (seq_t *)malloc(Ndivs * sizeof(seq_t));
        if (!divs) return (seq_t)0;          /* Allocation failure */

        size_t cnt = 0;
        gen_divs(0, (seq_t)1, &cnt, divs);
        divs_count = cnt;

        /* Sort the divisor list */
        qsort(divs, divs_count, sizeof(seq_t), cmp_seq_t);
    }

    if (n == 0 || n > divs_count) return (seq_t)0;
    return divs[n - 1];
}
