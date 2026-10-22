/*
 * A364469  Numbers k whose digital sum is greater than the number of primes
 *          up to k (inclusive).
 * https://oeis.org/A364469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th member of OEIS A364469,
   the numbers k whose digit sum exceeds the number of primes ≤ k.
   The sequence is finite; all members are found by sieving up to
   a fixed limit (10 000) and stored in a static array.
   a(n) is looked up from that array. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"   /* declares seq_t digit_sum(seq_t) */

#define LIMIT 10000   /* safe upper bound – after this no more terms exist */

static bool initialized = false;
static bool is_composite[LIMIT + 1];
static seq_t pi[LIMIT + 1];
static seq_t terms[LIMIT + 1];
static size_t term_count = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise sieve */
        is_composite[0] = true;
        is_composite[1] = true;
        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (!is_composite[(size_t)i]) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
        /* prefix counts of primes */
        seq_t cnt = 0;
        pi[0] = 0;
        for (idx_t i = 1; i <= LIMIT; ++i) {
            if (!is_composite[(size_t)i]) ++cnt;
            pi[(size_t)i] = cnt;
        }
        /* collect qualifying numbers */
        for (idx_t k = 1; k <= LIMIT; ++k) {
            seq_t dsum = digit_sum((seq_t)k);
            if (dsum > pi[(size_t)k]) {
                terms[term_count++] = (seq_t)k;
            }
        }
        initialized = true;
    }
    if (n == 0 || n > term_count) return 0;
    return terms[(size_t)(n - 1)];
}
