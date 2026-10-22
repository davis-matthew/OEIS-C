/*
 * A365872  The "double commas" sequence, a variant of A121805. See the
 *          Comments and Example sections for detailed explanations.
 * https://oeis.org/A365872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A365872: a(1)=1.
   a(n) is the smallest integer > a(n‑1) such that
   a(n) - a(n‑1) = 2 * (10 * last_digit(a(n‑1)) + first_digit(a(n))).
   The sequence stops when no such integer exists. */
 
#include <stdint.h>  /* for size_t, if desired */
 
/* Return the first decimal digit of x. */
static seq_t first_digit(seq_t x) {
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return x;
}
 
seq_t a(idx_t n) {
    /* Handle the offset. Undefined for n<1; return 0 for safety. */
    if (n < (idx_t)1) return (seq_t)0;
 
    seq_t prev = (seq_t)1;   /* a(1) */
    if (n == (idx_t)1) return prev;
 
    /* Compute terms up to a(n) */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t last = prev % (seq_t)10;          /* last decimal digit of prev */
        seq_t best = (seq_t)0;                  /* 0 means no candidate yet */
 
        for (idx_t d = (idx_t)1; d <= (idx_t)9; ++d) {
            seq_t candidate = prev + (seq_t)2 *
                              (((seq_t)10 * last) + (seq_t)d);
            /* Check that the first digit of the candidate is indeed d. */
            seq_t fd = first_digit(candidate);
            if (fd != (seq_t)d) continue;
 
            if (best == (seq_t)0 || candidate < best) {
                best = candidate;
            }
        }
 
        if (best == (seq_t)0) {          /* sequence ends before reaching n */
            return (seq_t)0;
        }
        prev = best;
    }
    return prev;
}
