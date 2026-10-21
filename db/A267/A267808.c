/*
 * A267808  a(0) = a(1) = 1; for n>1, a(n) = (a(n-1) mod 4) + a(n-2).
 * https://oeis.org/A267808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor((n+1)/3) + ((-1)^n - (-1)^(floor(n/3)))/2 + 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using signed 64‑bit arithmetic for the intermediate results.
       seq_t is unsigned in the build, but the final value is always non‑negative. */
    long long n_ll = (long long)n;
    long long floor_n1_3 = (n_ll + 1) / 3;
    long long res_ll = n_ll - floor_n1_3;

    int sign_n = (n_ll & 1) ? -1 : 1;              /* (-1)^n */
    int sign_k = ((n_ll / 3) & 1) ? -1 : 1;       /* (-1)^floor(n/3) */
    int diff = sign_n - sign_k;                   /* -2, 0 or 2 */
    long long add = diff / 2;                     /* -1, 0 or 1 */

    res_ll += add + 1;                            /* +1 from the formula */

    return (seq_t)res_ll;
}
