/*
 * A352531  Numbers with multiplicative persistence value 10.
 * https://oeis.org/A352531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th lexicographic permutation (ascending) of the
 *            multiset {3,7,7,8,8,8,8,9,9,9}
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352531: Numbers with multiplicative persistence value 10.
   The sequence consists of all 10‑digit numbers whose multiset of
   digits is {3,7,7,8,8,8,8,9,9,9}.  These numbers are sorted
   numerically; a(1) = 3778888999. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Digit multiplicities: 3×1, 7×2, 8×4, 9×3 */
    int cnt[10] = {0};
    cnt[3] = 1;
    cnt[7] = 2;
    cnt[8] = 4;
    cnt[9] = 3;

    /* Pre‑computed factorials up to 10! (stored in seq_t for
       arbitrary‑precision builds). */
    static const seq_t fact[11] = {
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)6,  (seq_t)24,
        (seq_t)120, (seq_t)720, (seq_t)5040, (seq_t)40320,
        (seq_t)362880, (seq_t)3628800
    };

    seq_t result = 0;
    int remaining = 10;          /* digits left to place */
    idx_t idx = n;               /* 1‑based index within the current prefix */

    for (int pos = 0; pos < 10; ++pos) {
        for (int d = 0; d <= 9; ++d) {
            if (cnt[d] == 0)
                continue;

            /* Tentatively place digit d at this position */
            cnt[d]--;
            int rem = remaining - 1;

            /* Count how many permutations start with this choice */
            seq_t perms = fact[rem];
            for (int j = 0; j <= 9; ++j)
                perms /= fact[cnt[j]];

            /* perms is guaranteed to fit in idx_t (max 10! = 3 628 800) */
            idx_t perms_idx = (idx_t)perms;

            if (perms_idx >= idx) {
                /* d is the correct digit at this position */
                result = result * (seq_t)10 + (seq_t)d;
                remaining = rem;
                break;
            } else {
                /* Skip all permutations that start with digit d */
                idx -= perms_idx;
                cnt[d]++;          /* restore count for next trial */
            }
        }
    }

    return result;
}
