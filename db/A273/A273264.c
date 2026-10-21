/*
 * A273264  Volume of unit n-ball, rounded to the nearest integer.
 * https://oeis.org/A273264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((Pi^(n/2)) / Gamma(1 + n/2))
 * Range:     u8 n <= 2, u16 n <= 1, u32 n <= 2, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fixed‑point scale: 10⁶ is large enough to guarantee correct
       rounding up to n = 10 000, but small enough that all 64‑bit
       intermediate products fit in uint64_t.                         */
    const seq_t SCALE     = (seq_t)1000000ULL;   /* 10⁶      */
    const seq_t PI_SCALED = (seq_t)3141593ULL;   /* Pi × SCALE rounded to nearest integer */

    if (n == 0) return 0;                      /* safety – out of range */

    /* The recurrence V_{n+2} = V_n * 2π / (n+2) is used.
       Values are kept in fixed‑point form, scaled by SCALE.        */
    seq_t cur;

    if (n % 2 == 1) {                          /* odd n – start from V₁ = 2 */
        cur = SCALE * (seq_t)2;                 /* 2 × SCALE */
        for (idx_t i = 3; i <= n; i += 2) {
            seq_t num = cur * (seq_t)2 * PI_SCALED;
            seq_t den = SCALE * (seq_t)i;
            cur = (num + den / (seq_t)2) / den;
        }
    } else {                                   /* even n – start from V₂ = π */
        cur = PI_SCALED;
        for (idx_t i = 4; i <= n; i += 2) {
            seq_t num = cur * (seq_t)2 * PI_SCALED;
            seq_t den = SCALE * (seq_t)i;
            cur = (num + den / (seq_t)2) / den;
        }
    }

    /* Round the fixed‑point value to the nearest integer. */
    return (cur + SCALE / (seq_t)2) / SCALE;
}
