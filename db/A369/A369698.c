/*
 * A369698  AGM transform of positive cubes.
 * https://oeis.org/A369698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2*(n+1)^2/4)^n - n^n * (n!)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 116 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A369698_pow(seq_t b, idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        if (e & 1)
            r *= b;
        e >>= 1;
        if (e > 0)
            b *= b;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t tri = ns * (ns + 1) / 2;
    seq_t cubes = tri * tri;               /* A000537(n) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; i++)
        fact = fact * (seq_t)i;
    seq_t f3 = fact * fact * fact;         /* A000442(n) */
    return A369698_pow(cubes, n) - A369698_pow(ns, n) * f3;
}
