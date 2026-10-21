/*
 * A272586  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 507", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (11*4^n - 12*2^n - 8)/3 for n > 1, a(0) = 1, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_local(idx_t exp)
{
    /* Compute 2^exp using repeated multiplication.
       Works for all seq_t types (fixed‑width or arbitrary‑precision). */
    seq_t val = 1;
    seq_t two = 2;
    for (idx_t i = 0; i < exp; ++i)
        val = val * two;
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;

    idx_t exp2 = n + n;          /* 2·n, i.e. exponent for 4^n */
    seq_t pow2_n  = pow2_local(n);    /* 2^n    */
    seq_t pow2_2n = pow2_local(exp2); /* 4^n = 2^(2n) */

    seq_t num = (seq_t)11 * pow2_2n - (seq_t)12 * pow2_n - (seq_t)8;
    return num / 3;
}
