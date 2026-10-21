/*
 * A255608  Numbers n such that 36n+11, 36(n+1)+11, 36(n+2)+11 and 36(n+3)+11
 *          are prime.
 * https://oeis.org/A255608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A255608:
   the n-th integer N such that 36 N+11, 36(N+1)+11, 36(N+2)+11 and
   36(N+3)+11 are all prime.  The function mimics a simple search
   increasing by 5 (since all terms are multiples of 5) and memoises
   previously found terms in a static dynamic array.  It uses the
   provided primality test is_prime_big(seq_t) and the utility
   grow_seq to enlarge the array on demand. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* handle the unfortunate case where the caller asks for 0 */
    if (n == 0) return 0;

    static seq_t *terms = NULL;           /* memoised terms */
    static size_t terms_len = 0;          /* current number of stored terms */
    static size_t terms_cap = 0;          /* allocated capacity */
    static seq_t next_candidate = 5;      /* next multiple of 5 to test */

    /* extend the stored list until we have at least n terms */
    while (terms_len < n) {
        seq_t k = next_candidate;                 /* candidate N */
        seq_t p0 = 36 * k + 11;                    /* 36k+11   */
        seq_t p1 = p0 + 36;                        /* 36(k+1)+11 */
        seq_t p2 = p1 + 36;                        /* 36(k+2)+11 */
        seq_t p3 = p2 + 36;                        /* 36(k+3)+11 */

        if (is_prime_big(p0) && is_prime_big(p1) &&
            is_prime_big(p2) && is_prime_big(p3)) {
            /* store the found term */
            grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = k;
        }

        /* move to the next multiple of 5 */
        next_candidate += 5;
    }

    return terms[n - 1];
}
