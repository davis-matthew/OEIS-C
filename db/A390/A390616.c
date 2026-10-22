/*
 * A390616  Sum of depths of leaves in a complete ternary tree with n nodes.
 * https://oeis.org/A390616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (h-1)*L + (n - M), where h = floor(log_3(2*n - 1)), M =
 *            (3^h - 1)/2, L = n - floor((n+1)/3)
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic with seq_t and idx_t types. */

seq_t a(idx_t n)
{
    /* Offset 1: return 0 for n = 1 (sequence starts at a(1)=0). */
    if (n <= 1)
        return (seq_t)0;

    /* Compute bound = 2*n - 1 as seq_t to avoid overflow. */
    seq_t bound = (seq_t)n * (seq_t)2 - (seq_t)1;

    /* Find h = floor(log_3(bound)) and 3^h as pow3. */
    seq_t pow3 = (seq_t)1;
    idx_t h = 0;
    while (pow3 * (seq_t)3 <= bound) {
        pow3 *= (seq_t)3;
        ++h;
    }

    /* Compute M = (3^h - 1) / 2. */
    seq_t M = (pow3 - (seq_t)1) / (seq_t)2;

    /* Compute L = n - floor((n+1)/3). */
    idx_t q = (n + (idx_t)1) / (idx_t)3;   /* floor((n+1)/3) */
    seq_t L = (seq_t)n - (seq_t)q;

    /* Compute the final value: (h-1)*L + (n - M). */
    seq_t term = (seq_t)(h - (idx_t)1) * L + ((seq_t)n - M);
    return term;
}
