/*
 * A285182  L = A132468(n) = A048669(n)-1 is the length of the longest run of
 *          consecutive numbers that have a common factor with n; a(n) =
 *          smallest k >= 0 which starts such a run.
 * https://oeis.org/A285182
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For a given integer n ≥ 2, let P be the set of distinct
   prime divisors of n and M = ∏ p∈P p (i.e., rad(n)).  An integer k shares
   a non‑trivial factor with n iff k ≡ 0 (mod p) for some p∈P.  Thus the
   pattern of such integers repeats with period M.  The longest run of
   consecutive integers all sharing a factor with n is the longest streak
   of "covered" residues in one or two consecutive periods.  The earliest
   start of such a run is the required a(n).  The implementation
   constructs a boolean array of size M marking covered residues, scans
   a doubled array to find the maximal run, and returns its minimal
   starting index. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is defined for n ≥ 2.  For n < 2
       the value is undefined; return 0 as a safe default. */
    if (n < 2) return (seq_t)0;

    /* Factor n to obtain its distinct prime divisors.  The factor_u64
       routine returns primes and their exponents; we ignore exponents. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    if (cnt == 0) return (seq_t)0;   /* n = 1, should not happen */

    /* Compute M = rad(n), the product of distinct primes. */
    size_t M = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* Every prime appears at least once in the factor table. */
        M *= (size_t)primes[i];
    }

    /* Allocate an array marking covered residues modulo M. */
    uint8_t *covered = (uint8_t*)calloc(M, sizeof(uint8_t));
    if (!covered) return (seq_t)0;  // allocation failure handling

    /* Mark all multiples of each prime as covered. */
    for (size_t i = 0; i < cnt; ++i) {
        size_t p = (size_t)primes[i];
        for (size_t j = 0; j < M; j += p) {
            covered[j] = 1;
        }
    }

    /* Scan a doubled period to find the longest consecutive run of
       covered residues, remembering the earliest start.  Because the
       pattern repeats with period M, the longest run can span the
       boundary between two periods. */
    size_t best_len = 0;
    size_t best_start = 0;
    size_t cur_len = 0;
    size_t cur_start = 0;

    for (size_t i = 0; i < 2 * M; ++i) {
        if (covered[i % M]) {
            if (cur_len == 0) cur_start = i;
            ++cur_len;
        } else {
            if (cur_len > best_len) {
                best_len = cur_len;
                if (cur_start < M) best_start = cur_start;
                else best_start = cur_start - M; /* map back to first period */
            }
            cur_len = 0;
        }
    }
    /* Handle case where the run ends at the very last element (i = 2*M-1). */
    if (cur_len > best_len) {
        best_len = cur_len;
        if (cur_start < M) best_start = cur_start;
        else best_start = cur_start - M;
    }

    free(covered);
    return (seq_t)best_start;
}
