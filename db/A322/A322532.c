/*
 * A322532  Position of 1/2^n in the sequence of all numbers 1/2^m, 1/3^m,
 *          2/3^m arranged in decreasing order.
 * https://oeis.org/A322532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·log₂(2)/log₂(3)⌋ + ⌊(n+1)·log₂(2)/log₂(3)⌋
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All calculations are performed in integer arithmetic; the logarithm
   ratio is pre‑computed as a long double constant with sufficient
   precision to avoid rounding errors for n ≤ 1e4. */
seq_t a(idx_t n)
{
    /* log(2)/log(3) = log₃(2) ≈ 0.630929753571457437786242285 */
    static const long double LOG_RATIO = 0.630929753571457437786242285L;

    /* Compute the two floor terms using long double precision. */
    idx_t f1 = (idx_t)((long double)n * LOG_RATIO);
    idx_t f2 = (idx_t)((long double)(n + 1ULL) * LOG_RATIO);

    /* Result is the sum of n and the two floor terms. */
    return (seq_t)n + f1 + f2;
}
