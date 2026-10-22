/*
 * A321786  Decimal expansion of wavelength of the red line of cadmium in dry
 *          air at 15 degrees Celsius and 760 mmHg under a gravity of 9.8067
 *          m/sec^2 in angström, which defined the angström unit from 1907 to
 *          1960.
 * https://oeis.org/A321786
 *
 * Offset:    -6    (a(-6) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = the (n+6)-th decimal digit of 64384696 (most significant
 *            digit at n = -6). For n > 1 all terms are 0. The sequence
 *            terminates with zeros after the last digit of 6438.4696
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constant 6438.4696 represented as integer 64384696. */
    static const seq_t DOC = 64384696ULL;
    /* Powers of ten 10^0 ... 10^7. */
    static const seq_t POW10[8] = {1ULL,10ULL,100ULL,1000ULL,10000ULL,
                                   100000ULL,1000000ULL,10000000ULL};

    /* For n outside the defined range produce 0 (ignored for n < -6). */
    if (n < -6 || n > 1) return 0;

    /* Map n to index 0..7 where 0 corresponds to the most significant
       digit. */
    int idx = (int)(n + 6);               /* idx ∈ [0,7] */
    int pow_idx = 7 - idx;                /* 10^(7-idx) */

    return (DOC / POW10[pow_idx]) % 10;
}
