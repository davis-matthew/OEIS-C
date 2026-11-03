/*
 * A379419  a(n) = n + [n*r/t] + [n*s/t], where r = (3/2)^(1/4); s =
 *          (3/2)^(1/2); t = (3/2)^(3/4) and [ ] = floor.
 * https://oeis.org/A379419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/r⌋ + ⌊n/r²⌋, where r = (3/2)^(1/4)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes ⌊n / r⌋, where r^4 = 3/2 */
static seq_t floor_div_by_r(seq_t n) {
    seq_t n_sq = n * n;
    seq_t n_four = n_sq * n_sq;          /* n^4  */
    seq_t target = (seq_t)2 * n_four;    /* 2 * n^4 */

    idx_t low = 0;
    idx_t high = (idx_t)n;               /* floor(n/r) <= n */

    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        seq_t mid2 = (seq_t)mid * mid;
        seq_t mid4 = mid2 * mid2;        /* mid^4 */
        seq_t val = (seq_t)3 * mid4;     /* 3 * mid^4 */

        if (val <= target) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)high;                  /* largest m with 3*m^4 <= 2*n^4 */
}

/* Computes ⌊n / r²⌋, where r² = sqrt(3/2) */
static seq_t floor_div_by_r2(seq_t n) {
    seq_t n_sq = n * n;                  /* n^2  */
    seq_t target = (seq_t)2 * n_sq;      /* 2 * n^2 */

    idx_t low = 0;
    idx_t high = (idx_t)n;               /* floor(n/r²) <= n */

    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        seq_t mid2 = (seq_t)mid * mid;   /* mid^2 */
        seq_t val = (seq_t)3 * mid2;     /* 3 * mid^2 */

        if (val <= target) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)high;                  /* largest m with 3*m^2 <= 2*n^2 */
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t f1 = floor_div_by_r(n_seq);
    seq_t f2 = floor_div_by_r2(n_seq);
    return n_seq + f1 + f2;
}
