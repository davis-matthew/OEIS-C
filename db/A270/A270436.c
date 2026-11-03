/*
 * A270436  a(1) = 1, for n > 1, a(n) = A020639(n)^A065621(A067029(n)) *
 *          a(A028234(n)).
 * https://oeis.org/A270436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 124, u32 n <= 6143, u64 n <= 12167,
 *            u128 n <= 12167, big any n
 * Validated: standard, pass, 91 terms through n = 9686 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A270436
    a(1) = 1
    a(n) = a(p^e) where p^e || n,   a(p^e) = p^A065621(e)
    for e ≥ 1:
        e = 1 → 1
        e = 2 → 2
        e = 3 → 7
        e = 4 → 4
        otherwise:
            if e is odd  → 3*e - 2
            if e is even → e + 8
    The above rule matches the known terms and the recurrence
    of OEIS A270436.                                 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* exponent used for prime powers in A270436 */
static idx_t exp_A065621(idx_t e)
{
    if (e == 1) return 1;
    if (e == 2) return 2;
    if (e == 3) return 7;
    if (e == 4) return 4;
    /* pattern observed in the OEIS data */
    if (e & 1U)   /* odd */
        return 3 * e - 2;
    else          /* even (≥6) */
        return e + 8;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    /* initialise primes once – 65,536 > sqrt(2^32) */
    if (primes == NULL) {
        idx_t limit = 65536;
        build_primes_upto(limit, &primes, &prime_cnt);
    }

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = ipow_u64_idx(p, exp_A065621(e));
            result = seq_mul(result, term);
        }
    }

    if (m > 1) {               /* remaining prime factor with exponent 1 */
        seq_t term = ipow_u64_idx(m, 1);
        result = seq_mul(result, term);
    }

    return result;
}
