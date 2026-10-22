/*
 * A303589  Floor(n*alpha)-1, where alpha is the number with continued
 *          fraction expansion [1;1,2,3,4,5,...] (A247844).
 * https://oeis.org/A303589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*alpha) - 1 where alpha is the number with
 *            continued fraction expansion [1;1,2,3,4,5,…]
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* value of floor(alpha * 1e15)  (determined with high‑precision arithmetic) */
static const seq_t ALPHA_SCALED = 1697770677599947ULL;
static const seq_t SCALE      = 1000000000000000ULL;

/* compute the sequence term a(n) */
seq_t a(idx_t n)
{
    seq_t num = ((seq_t)n) * ALPHA_SCALED; /* n * alpha_scaled    */
    seq_t val = num / SCALE;              /* floor(n * alpha)    */
    return val - (seq_t)1;                /* subtract 1           */
}
