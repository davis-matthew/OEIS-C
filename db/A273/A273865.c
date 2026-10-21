/*
 * A273865  The successive numbers of integers visible between two odd terms
 *          are given by the sequence itself.
 * https://oeis.org/A273865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is built by the following construction:
        start with 0; thereafter the odds 1,3,5,… are inserted,
        each followed by a block of the smallest unused even numbers.
        The length of the block after odd O_k is a(k) – the value
        of the sequence at index k.  The algorithm below implements
        this rule in O(n) time.                                        */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1 – unused case */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq) return 0;                  /* out of memory – return 0 */

    seq_t next_even   = 0;                /* next unused even number   */
    seq_t odd_value   = 1;                /* next odd to place (2*oddk-1) */
    idx_t odd_ordinal = 1;                /* ordinal of current odd   */
    seq_t next_odd_pos = (seq_t)2;        /* position of next odd (index 2) */

    for (idx_t i = 1; i <= n; ++i) {
        if ((seq_t)i == next_odd_pos) {   /* place odd number          */
            seq[i] = odd_value;
            /* compute position of following odd, using a(odd_ordinal) */
            next_odd_pos = (seq_t)i + (seq_t)1 + seq[odd_ordinal];
            odd_value += (seq_t)2;
            odd_ordinal++;
        } else {                          /* place next smallest even  */
            seq[i] = next_even;
            next_even += (seq_t)2;
        }
    }
    seq_t result = seq[n];
    free(seq);
    return result;
}
