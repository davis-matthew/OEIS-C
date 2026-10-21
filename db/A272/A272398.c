/*
 * A272398  The union of hexagonal numbers (A000384) and centered 9-gonal
 *          numbers (A060544).
 * https://oeis.org/A272398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 241, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A272398: union of hexagonal numbers H_k = k*(2*k-1) 
 *  and centered 9-gonal numbers C_k = (9*k*k - 9*k + 2)/2.
 *  The sequence is sorted in increasing order without duplicates.
 *  a(1) = 1, a(2) = 6, a(3) = 10, a(4) = 15, …
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1, undefined for n=0 */

    idx_t i = 1, j = 1;                   /* indices for H and C */
    seq_t h = (seq_t)i * ((seq_t)(2 * i - 1));           /* H_1 = 1   */
    seq_t c = ((seq_t)9 * j * j - (seq_t)9 * j + (seq_t)2) / 2; /* C_1 = 1 */
    idx_t count = 1;                       /* how many terms produced so far */
    seq_t result = h;                      /* placeholder */

    if (n == 1) return result;             /* first term is 1 */

    while (count < n) {
        /* compute next values when needed */
        if (h < c) {
            /* hexagonal number is next unique term */
            ++i;
            h = (seq_t)i * ((seq_t)(2 * i - 1));
        } else if (c < h) {
            /* centered 9‑gonal number is next unique term */
            ++j;
            c = ((seq_t)9 * j * j - (seq_t)9 * j + (seq_t)2) / 2;
        } else {                /* h == c : common term, output once */
            ++i;
            h = (seq_t)i * ((seq_t)(2 * i - 1));
            ++j;
            c = ((seq_t)9 * j * j - (seq_t)9 * j + (seq_t)2) / 2;
        }
        ++count;
        result = (h < c) ? h : ((c < h) ? c : h); /* store new term */
    }
    return result;
}
