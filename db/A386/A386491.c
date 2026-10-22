/*
 * A386491  Array read by ascending antidiagonals: A(n,m) =
 *          denominator(2n*m/(m^2 + 1)), where m > 0.
 * https://oeis.org/A386491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   a(n) = denominator of the reduced fraction 2*i*j / (j*j + 1)
 *            where i,j are the (n,m) indices obtained by reading the matrix
 *            A(i,j) = denominator(2*i*j/(j*j + 1)) in ascending antidiagonals
 *            with decreasing i on each antidiagonal
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean gcd for seq_t. */
static seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* sequence is undefined for n<1 */
    }

    /* Find the diagonal sum k (i+j) that contains the n-th element. */
    idx_t k = 2;
    while (((seq_t)k * (k - 1) / 2) < (seq_t)n) {
        ++k;
    }

    /* Number of elements before this diagonal. */
    seq_t prev = ((seq_t)(k - 2) * (k - 1)) / 2;
    idx_t offset = (idx_t)((seq_t)n - prev - 1);  /* zero‑based offset in the diagonal */

    /* Indices i (n) and j (m). */
    idx_t i = (k - 1) - offset;   /* decreasing i order */
    idx_t j = k - i;

    /* Compute numerator and denominator of the fraction. */
    seq_t numerator   = (seq_t)2 * i * j;
    seq_t denominator = (seq_t)j * j + 1;

    /* Reduce the fraction. */
    seq_t g = _gcd_seq_t(numerator, denominator);
    return denominator / g;
}
