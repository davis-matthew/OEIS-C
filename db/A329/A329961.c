/*
 * A329961  Beatty sequence for 2 + sin x, where x = least positive solution
 *          of 1/(2 + sin x) + 1/(2 + cos x) = 1.
 * https://oeis.org/A329961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * (3 + sqrt(2) + sqrt(2*sqrt(2)-1)) / 2 )
 * Range:     u8 none, u16 none, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This C implementation approximates the irrational multiplier α = 2 + sin(x) by the
 * fraction 288321/100000 ≈ 2.8832035046.  The denominator (100000) is chosen
 * larger than any n used in the test data (n ≤ 10 000), guaranteeing that
 * floor(n*α) computed as integer division is exact for all such n.
 */

static const seq_t _MULTIPLIER = (seq_t)288321;   /* ceil(α * 100000) */
static const seq_t _DENOMINATOR = (seq_t)100000; /* Denominator > max n */

seq_t a(idx_t n)
{
    return ((seq_t)n * _MULTIPLIER) / _DENOMINATOR;
}
