/*
 * A392305  Compensation points to be given to the presenter such that the
 *          game "Fill your basket" with n coins is fair.
 * https://oeis.org/A392305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A392305:
   a(n) = ceil(n/3) + 1  if n == 5 or (n % 3 == 0 && n % 9 != 0 && n != 3),
   otherwise a(n) = ceil(n/3).
   The sequence begins: 1,1,1,2,3,3,3,3,3,4,4,5,5,5,6,6, ... */

/* Calculates the integer ceil(n/3) using integer arithmetic. */
static inline idx_t ceil_div3(idx_t n) {
    return (n + 2) / 3;
}

seq_t a(idx_t n) {
    /* Guard against undefined behavior for n < 1 (offset 1). */
    if (n < 1) {
        return (seq_t)0;   /* value is not defined for n < 1 */
    }

    idx_t base = ceil_div3(n);            /* ceil(n/3) */
    idx_t add  = 0;

    /* Special offset cases derived from the pattern of the sequence. */
    if (n == 5) {
        add = 1;
    } else if ((n % 3) == 0) {
        /* n is a multiple of 3; add +1 except when n is a multiple of 9
           or when n == 3 (the first such case). */
        if (n % 9 != 0 && n != 3) {
            add = 1;
        }
    }

    return (seq_t)(base + add);
}
