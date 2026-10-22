/*
 * A304561  Number of minimum total dominating sets in the n-triangular
 *          (Johnson) graph.
 * https://oeis.org/A304561
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3k+1)!/(2^k·k!) if n = 3k+1 4k·(3k+2)!/(3·2^k·k!) if n =
 *            3k+2 (18-11k-21k²+32k³)(3k)!/(18·2^k·k!) if n = 3k with k =
 *            ⌊n/3⌋ and the corresponding remainder
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;

    idx_t r = n % 3;
    idx_t k = (r == 0) ? n / 3 : (n - r) / 3;   /* k as defined above */

    /* helper to compute 2^k using shifts */
    /* not needed explicitly, we remove factors of 2 while multiplying */

    seq_t result = 1;
    seq_t pow2_rem = k;    /* remaining power of two to divide */

    if (r == 1) {                  /* n = 3k+1 :  (3k+1)!/(2^k k!) */
        for (idx_t i = k + 1; i <= 3 * k + 1; ++i) {
            seq_t v = (seq_t)i;
            while (pow2_rem && (v & 1) == 0) {
                v >>= 1;
                --pow2_rem;
            }
            result *= v;
        }
        while (pow2_rem) result >>= 1;   /* any leftover 2's */
        return result;
    }

    if (r == 2) {                  /* n = 3k+2 : 4k(3k+2)!/(3·2^k·k!) */
        for (idx_t i = k + 1; i <= 3 * k + 2; ++i) {
            seq_t v = (seq_t)i;
            while (pow2_rem && (v & 1) == 0) {
                v >>= 1;
                --pow2_rem;
            }
            result *= v;
        }
        while (pow2_rem) result >>= 1;   /* any leftover 2's */

        result *= 4;        /* multiply by 4 */
        result *= (seq_t)k;  /* multiply by k */
        result /= 3;         /* divide by 3 */
        return result;
    }

    /* r == 0 : n = 3k : (18-11k-21k²+32k³)(3k)!/(18·2^k·k!) */
    for (idx_t i = k + 1; i <= 3 * k; ++i) {
        seq_t v = (seq_t)i;
        while (pow2_rem && (v & 1) == 0) {
            v >>= 1;
            --pow2_rem;
        }
        result *= v;
    }
    while (pow2_rem) result >>= 1;   /* any leftover 2's */

    /* compute polynomial factor */
    seq_t kk = (seq_t)k;
    seq_t poly = (seq_t)18
                - (seq_t)11 * kk
                - (seq_t)21 * kk * kk
                + (seq_t)32 * kk * kk * kk;

    result *= poly;      /* multiply by polynomial */
    result /= 18;        /* divide by 18 */
    return result;
}
