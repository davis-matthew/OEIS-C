/*
 * A329517  Expansion of (2*x^6 - 3*x^4 - 3*x^3 - 3*x^2 - 2*x - 1)/(x - 1).
 * https://oeis.org/A329517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329517: G.f. = (2*x^6 - 3*x^4 - 3*x^3 - 3*x^2 - 2*x - 1)/(x-1).
   Recurrence: a(0)=1; for n>=1, a(n)=a(n-1)-coeff[n], coeff[1..6]={-2,-3,-3,-3,0,2}. */

seq_t a(idx_t n)
{
    static const seq_t delta[7] = {0, 2, 3, 3, 3, 0, (seq_t)-2};
    seq_t res = 1;            /* a(0) */
    if (n == 0) return res;   /* already correct */

    /* Add deltas up to min(n,6). For k>6 the delta is zero. */
    idx_t limit = n < 6 ? n : 6;
    for (idx_t k = 1; k <= limit; ++k)
        res += delta[k];

    return res;
}
