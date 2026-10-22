/*
 * A338953  Number of unoriented colorings of the 96 edges (or triangular
 *          faces) of the 4-D 24-cell using subsets of a set of n colors.
 * https://oeis.org/A338953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (96*n^8 + 144*n^12 + 144*n^16 + 128*n^18 + 192*n^20 +
 *                228*n^24 + 48*n^32 + 32*n^36 + 43*n^48 + 72*n^50 +
 *                12*n^52 + 12*n^60 + n^96) / 1152
 *
 * Each term is computed using integer arithmetic.  The helper function
 * pow_seq_local implements repeated‑squaring on a seq_t value.
 */

#include <stdint.h>
#include <stdbool.h>

/* local power function: base^exp for seq_t */
static inline seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum   = 0;
    seq_t base  = (seq_t)n;

    sum += 96L  * pow_seq_local(base, 8);
    sum += 144L * pow_seq_local(base, 12);
    sum += 144L * pow_seq_local(base, 16);
    sum += 128L * pow_seq_local(base, 18);
    sum += 192L * pow_seq_local(base, 20);
    sum += 228L * pow_seq_local(base, 24);
    sum += 48L  * pow_seq_local(base, 32);
    sum += 32L  * pow_seq_local(base, 36);
    sum += 43L  * pow_seq_local(base, 48);
    sum += 72L  * pow_seq_local(base, 50);
    sum += 12L  * pow_seq_local(base, 52);
    sum += 12L  * pow_seq_local(base, 60);
    sum +=     pow_seq_local(base, 96);

    return sum / 1152L;
}
