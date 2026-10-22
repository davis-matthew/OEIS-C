/*
 * A387306  Primes p == 3 (mod 4) such that neither 2p-1 nor 2p+3 is a sum of
 *          two squares.
 * https://oeis.org/A387306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387306: primes p ≡ 3 (mod 4) for which neither 2p‑1 nor 2p+3
   is a sum of two squares. The order of evaluation is unspecified;
   the function memoises previously computed values. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static seq_t *terms = NULL;   /* memoised primes satisfying the property */
static size_t terms_count = 0;
static size_t terms_cap   = 0;
static idx_t  next_candidate = 3;   /* odd numbers ≥3 */

static inline bool is_sum_two_squares(seq_t n)
{
    uint64_t m = (uint64_t)(n);    /* all test numbers fit in 64 bits */
    /* Remove factors of 2 – they do not affect the condition */
    while ((m & 1ULL) == 0) m >>= 1;

    uint64_t d = 3;
    while (d * d <= m) {
        if (m % d == 0) {
            unsigned exp = 0;
            do {
                m /= d;
                ++exp;
            } while (m % d == 0);
            if ((d % 4ULL == 3ULL) && (exp & 1)) return false;
        }
        d += 2;
    }
    if (m > 1 && (m % 4ULL == 3ULL)) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* 0 is unused for the OEIS offset 1 */

    /* grow memoisation array as needed */
    while (terms_count < n) {
        /* Fast skip of even numbers */
        while (1) {
            if ((next_candidate & 1ULL) &&           /* odd */
                (next_candidate % 4ULL == 3ULL) &&    /* ≡3 (mod 4) */
                is_prime(next_candidate)) {
                break;                     /* found a candidate prime */
            }
            next_candidate += 2ULL;          /* next odd number */
        }

        /* test the two numbers 2p‑1 and 2p+3 */
        __uint128_t mp1 = (__uint128_t)next_candidate * 2ULL - 1ULL;
        __uint128_t mp2 = (__uint128_t)next_candidate * 2ULL + 3ULL;

        uint64_t m1 = (uint64_t)mp1;     /* safe, mp1 ≤ 2·max(idx_t) */
        uint64_t m2 = (uint64_t)mp2;

        if (!is_sum_two_squares((seq_t)m1) &&
            !is_sum_two_squares((seq_t)m2)) {
            /* store the prime */
            if (terms_count + 1 > terms_cap) {
                ensure_capacity(terms_count + 1, &terms_cap,
                                (void **)&terms, sizeof(seq_t));
            }
            terms[terms_count++] = (seq_t)next_candidate;
        }

        next_candidate += 2ULL;   /* advance to the next odd number */
    }

    return terms[n - 1];
}
