/*
 * A246078  Paradigm shift sequence for (-1,4) production scheme with
 *          replacement.
 * https://oeis.org/A246078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * a(n-11) for all n >= 26. The first 25 terms are given
 *            explicitly below: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5,
 *            a(6)=6, a(7)=7, a(8)=8, a(9)=9, a(10)=10, a(11)=11, a(12)=12,
 *            a(13)=13, a(14)=14, a(15)=16, a(16)=18, a(17)=20, a(18)=22,
 *            a(19)=24, a(20)=27, a(21)=30, a(22)=33, a(23)=36, a(...
 * Range:     u8 n <= 42, u16 n <= 97, u32 n <= 209, u64 n <= 431,
 *            u128 n <= 875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for standard integer types */

/* Helper: array of the first 25 terms (index 0 corresponds to a(1)). */
static const seq_t base_terms[25] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)4,  (seq_t)5,
    (seq_t)6,  (seq_t)7,  (seq_t)8,  (seq_t)9,  (seq_t)10,
    (seq_t)11, (seq_t)12, (seq_t)13, (seq_t)14, (seq_t)16,
    (seq_t)18, (seq_t)20, (seq_t)22, (seq_t)24, (seq_t)27,
    (seq_t)30, (seq_t)33, (seq_t)36, (seq_t)40, (seq_t)44
};

seq_t a(idx_t n) {
    /* Ensure n is within the expected range; behaviour below offset is undefined. */
    if (n <= 25) {
        return base_terms[n - 1];
    }

    /* For n >= 26, use the recurrence a(n) = 3 * a(n-11).
       Reduce modulo 11 steps, accumulating a multiplying factor of 3. */
    seq_t factor = (seq_t)1;
    idx_t m = n;
    while (m > 25) {
        factor *= (seq_t)3;
        m -= (idx_t)11;
    }
    /* m is now <= 25; return the base term multiplied by the factor. */
    return base_terms[m - 1] * factor;
}
