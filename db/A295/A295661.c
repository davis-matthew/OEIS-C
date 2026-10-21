/*
 * A295661  Numbers with at least one odd exponent larger than one in their
 *          prime factorization.
 * https://oeis.org/A295661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7761, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295661: Numbers with at least one odd exponent ≥ 3 in their prime factorization */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    static seq_t *seq   = NULL;   /* cache of found terms */
    static size_t len   = 0;      /* number of cached elements   */
    static size_t cap   = 0;      /* allocated capacity          */
    static uint64_t curr = 1;     /* last integer examined       */

    /* Expand cache until it contains at least n terms */
    while (len < n) {
        ++curr;   /* next integer to test           */

        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt;
        factor_u64(curr, primes, exps, &cnt);

        int ok = 0;
        for (size_t i = 0; i < cnt && !ok; ++i) {
            if ((exps[i] & 1ULL) && exps[i] >= 3ULL) {
                ok = 1;
            }
        }

        if (ok) {
            if (len == cap) {
                cap = cap ? cap * 2 : 4;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = (seq_t)curr;
        }
    }

    /* seq is 0‑based, a(1) is seq[0] */
    return seq[n - 1];
}
