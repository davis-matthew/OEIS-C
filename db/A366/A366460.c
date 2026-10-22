/*
 * A366460  Odd terms in A366825.
 * https://oeis.org/A366460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 3466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th odd integer that can be written as p²·m, where
   p is an odd prime, m > 1 is square‑free, and all prime factors of m
   are strictly larger than p.  The sequence starts with a(1) = 45. */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a 64‑bit integer satisfies the definition.  All
   arithmetic here stays within 64 bits, which is sufficient for
   the range of n considered by the tests. */
static bool is_good(uint64_t x)
{
    if ((x & 1ULL) == 0)          /* must be odd */
        return false;

    uint64_t temp = x;
    uint64_t p_sq   = 0;          /* the square prime factor, if found */
    uint64_t min_other = UINT64_MAX;
    bool has_other = false;       /* at least one other prime factor */

    /* Trial division by odd numbers up to sqrt(temp). */
    for (uint64_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            uint64_t exp = 0;
            while (temp % d == 0) {
                temp /= d;
                exp++;
                if (exp > 2)
                    return false;   /* exponent > 2 not allowed */
            }
            if (exp == 2) {
                if (p_sq != 0)      /* more than one squared prime */
                    return false;
                p_sq = d;
            } else { /* exp == 1 */
                if (!has_other || d < min_other) {
                    min_other = d;
                    has_other = true;
                }
            }
        }
    }

    /* Remaining prime factor (if any). */
    if (temp > 1) {
        if (!has_other || temp < min_other) {
            min_other = temp;
            has_other = true;
        }
    }

    /* Must contain exactly one squared prime and at least one other
       distinct larger prime factor. */
    if (p_sq == 0 || !has_other)
        return false;
    if (p_sq >= min_other)
        return false;

    return true;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    idx_t  count    = 0;      /* how many good numbers found */
    uint64_t candidate = 3;  /* first odd integer */

    while (1) {
        if (is_good(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;
    }
}
