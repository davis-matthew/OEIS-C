/*
 * A300760  Number of ways to select 4 numbers from the set of the first n
 *          natural numbers avoiding 3-term arithmetic progressions.
 * https://oeis.org/A300760
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^4 - 12 n^3 + 51 n^2 - 78 n + 32)/24) + ((n%2)?
 *            (-n+2)/4 : 0) + c(n) where c(n) = 0 for n ≡ 1,2,5,7,10,11 (mod
 *            12) = -1/3 for n ≡ 3,6,9 (mod 12) = -4/3 for n ≡ 0 (mod 12) = -1
 *            for n ≡ 4,8 (mod 12)
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 569, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) /* offset behaviour not required */
        return 0;

    /* use signed 128‑bit arithmetic to avoid overflow */
    __int128 nn   = n;
    __int128 poly = nn*nn*nn*nn
                  - (__int128)12*nn*nn*nn
                  + (__int128)51*nn*nn
                  - (__int128)78*nn
                  + 32;

    __int128 num = poly;

    /* add the term ((-n+2)/4) for odd n, expressed with denominator 24 */
    if (n & 1)
        num += (__int128)6 * (-nn + 2);

    /* add the c(n) contribution, also expressed with denominator 24 */
    int mod12 = n % 12;
    __int128 cnum = 0;
    if (mod12 == 0)
        cnum = -(__int128)32;                 /* -4/3  -> -32/24 */
    else if (mod12 == 4 || mod12 == 8)
        cnum = -(__int128)24;                 /* -1    -> -24/24 */
    else if (mod12 == 3 || mod12 == 6 || mod12 == 9)
        cnum = -(__int128)8;                  /* -1/3  -> -8/24 */
    /* else cnum = 0 for the remaining residues */

    num += cnum;

    /* final result: numerator divided by 24 (exact division) */
    return (seq_t)(num / 24);
}
