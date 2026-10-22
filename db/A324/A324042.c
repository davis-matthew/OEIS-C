/*
 * A324042  Number of triangular regions into which a figure made up of a row
 *          of n adjacent congruent rectangles is divided upon drawing
 *          diagonals of all possible rectangles.
 * https://oeis.org/A324042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A324042
 * Formula:
 *   a(n) = 2 * ( 2*n^2 - n + 1 + 2 * Σ_{i=2..⌊n/2⌋} (n+1-2*i) * (n+1-i) * φ(i) )
 *   where φ(i) is Euler's totient function.
 *
 * The implementation computes φ(i) on the fly using a simple
 * integer factorisation – adequate for n up to about 10⁴
 * (roughly 5 000 totient evaluations per call).  All arithmetic
 * is performed in seq_t, with integer conversions to idx_t only
 * when an index is needed (loop counters, etc.).  No external
 * state is required, so the function works correctly for any
 * calling order of n.
 */

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------
 *  Helper: compute φ(k) for k ≥ 2 using integer factorisation.
 * ------------------------------------------------------------ */
static seq_t phi_idx(idx_t k)
{
    if (k == 0) return 0;
    if (k == 1) return (seq_t)1;

    seq_t result = (seq_t)k;          /* start with φ(k) = k   */
    idx_t tmp = k;                    /* copy for factorisation */

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / (seq_t)p;   /* result = result * (1 - 1/p) */
        }
    }
    if (tmp > 1) {
        result -= result / (seq_t)tmp;
    }
    return result;
}

/* ------------------------------------------------------------
 *  Main sequence function.
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Convert n to seq_t for exact arithmetic. */
    const seq_t nseq   = (seq_t)n;
    const seq_t nplus1 = nseq + (seq_t)1;

    /* Base part: 2*n^2 - n + 1 */
    seq_t base = (seq_t)2 * nseq * nseq;   /* 2*n^2 */
    base -= nseq;                          /* - n   */
    base += (seq_t)1;                      /* + 1   */

    /* Sum part: Σ (n+1-2*i)(n+1-i)φ(i)  over i=2..⌊n/2⌋ */
    seq_t sum = 0;
    if (n >= 2) {
        const idx_t limit = n / 2;
        for (idx_t i = 2; i <= limit; ++i) {
            const seq_t ii      = (seq_t)i;
            const seq_t term1   = nplus1 - (seq_t)2 * ii;   /* n+1-2i  */
            const seq_t term2   = nplus1 - ii;             /* n+1-i   */
            const seq_t phi_i   = phi_idx(i);             /* φ(i)    */
            const seq_t prod    = term1 * term2 * phi_i;  /* product */
            sum += prod;
        }
    }

    /* Final value: 2 * ( base + 2*sum ) */
    seq_t result = base;
    result += (seq_t)2 * sum;          /* base + 2*sum */
    result *= (seq_t)2;                /* multiply by 2 */

    return result;
}
