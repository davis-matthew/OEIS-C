/*
 * A320390  Prime signature of n (sorted in decreasing order), concatenated.
 * https://oeis.org/A320390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concatenation of the exponents (sorted in decreasing order)
 * of the prime factorization of n.  For n = 1, a(1) = 0.
 *
 * Implementation uses the utility function factor_u64 to obtain the
 * prime exponents, sorts them, and concatenates their decimal
 * representations into a seq_t value.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Comparator for qsort: sort uint64_t in descending order */
static int cmp_desc(const void *va, const void *vb) {
    uint64_t a = *(const uint64_t *)va;
    uint64_t b = *(const uint64_t *)vb;
    if (a < b) return 1;
    if (a > b) return -1;
    return 0;
}

seq_t a(idx_t n) {
    if (n <= 1)                /* n == 1 (or 0, but offset starts at 1) */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];       /* storage for prime factors, large enough for all n ≤ 10⁸ */
    uint64_t exps[64];         /* corresponding exponents */
    size_t cnt = 0;

    /* factor_u64 writes the prime factors and exponents into the
       provided arrays and sets cnt to the number of distinct primes. */
    factor_u64(nn, primes, exps, &cnt);

    /* Sort exponents in decreasing order. */
    qsort(exps, cnt, sizeof(uint64_t), cmp_desc);

    /* Concatenate the decimal digits of each exponent. */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];

        /* Determine 10^digits(e). */
        seq_t pow10 = 1;
        uint64_t tmp = e;
        do {
            pow10 *= (seq_t)10;
            tmp /= 10;
        } while (tmp);

        res = res * pow10 + (seq_t)e;
    }

    return res;
}
