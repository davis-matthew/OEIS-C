/*
 * A253238  Number of ways to write n as a sum of a perfect power (>1) and a
 *          prime.
 * https://oeis.org/A253238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # {p | p is a perfect power >1, p ≤ n, and n–p is prime}
 * Range:     u8 n <= 63, u16 n <= 2639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* malloc, realloc, free, qsort */
#include <stdint.h>                 /* uint64_t, uint32_t */
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x) */

/* comparison function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/* count the number of ways to write n as a perfect power (>1) plus a   */
/* prime.  Perfect powers are numbers of the form m^e for integers    */
/* m≥2, e≥2.  The function works for any seq_t/idx_t provided by the   */
/* surrounding test harness.  The algorithm runs in O(sqrt(n) +   */
/* number_of_powers) time, which satisfies the 2‑s time requirement   */
/* for n up to about 10000.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 4)      /* smallest perfect power >1 is 2^2 = 4 */
        return 0;

    seq_t count = 0;
    seq_t sn = (seq_t)n;                /* cast n once for speed */

    /* collect all perfect powers p ≤ n, potentially with duplicates */
    uint64_t *powers = NULL;
    size_t len = 0, cap = 0;

    for (idx_t base = 2; ; ++base) {
        /* stop when base^2 would exceed n */
        seq_t base2 = (seq_t)base * (seq_t)base;
        if (base2 > sn)
            break;

        seq_t power = base2;
        while (power <= sn) {
            /* power ≤ n fits in idx_t and thus in uint64_t */
            uint64_t val = (uint64_t)(idx_t)power;

            /* grow array */
            if (len == cap) {
                cap = (cap == 0) ? 4 : cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(powers, cap * sizeof(uint64_t));
                if (!tmp) {          /* allocation failed – abort gracefully */
                    free(powers);
                    return count;
                }
                powers = tmp;
            }
            powers[len++] = val;

            /* next power: multiply by base (still a seq_t) */
            power = power * (seq_t)base;
        }
    }

    if (len == 0) {                      /* no perfect powers ≤ n */
        free(powers);
        return count;
    }

    /* sort and deduplicate the array of perfect powers */
    qsort(powers, len, sizeof(uint64_t), cmp_uint64);
    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (i == 0 || powers[i] != powers[i - 1]) {
            powers[uniq++] = powers[i];
        }
    }

    /* count primes in the complementary part */
    for (size_t i = 0; i < uniq; ++i) {
        uint64_t val = powers[i];
        idx_t rem = n - val;
        if (is_prime(rem))
            ++count;
    }

    free(powers);
    return count;
}
