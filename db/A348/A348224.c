/*
 * A348224  Lower matching number of the n-triangular honeycomb acute knight
 *          graph.
 * https://oeis.org/A348224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4)+a(n-12)-a(n-13)-a(n-15)+a(n-16)
 *            Initial values:
 *            a(1)=0,a(2)=0,a(3)=3,a(4)=3,a(5)=3,a(6)=6,a(7)=9,
 *            a(8)=9,a(9)=15,a(10)=18,a(11)=18,a(12)=24,
 *            a(13)=29,a(14)=30,a(15)=39,a(16)=44
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* store the first 16 terms (index 0 is unused but convenient) */
    static const seq_t init[17] = {
        0,  /* a(0) unused */
        0,  /* a(1) */
        0,  /* a(2) */
        3,  /* a(3) */
        3,  /* a(4) */
        3,  /* a(5) */
        6,  /* a(6) */
        9,  /* a(7) */
        9,  /* a(8) */
        15, /* a(9) */
        18, /* a(10) */
        18, /* a(11) */
        24, /* a(12) */
        29, /* a(13) */
        30, /* a(14) */
        39, /* a(15) */
        44  /* a(16) */
    };

    if (n == 0) return (seq_t)0;          /* out‑of‑range guard */
    if (n <= 16) return init[n];          /* return known initial value */

    /* ring buffer of 17 slots to keep values a[i-16] .. a[i] */
    seq_t buf[17];
    for (idx_t i = 0; i < 17; ++i) buf[i] = init[i];

    for (idx_t i = 17; i <= n; ++i) {
        seq_t pos = buf[(i-1)%17] + buf[(i-3)%17] + buf[(i-12)%17] + buf[(i-16)%17];
        seq_t neg = buf[(i-4)%17] + buf[(i-13)%17] + buf[(i-15)%17];
        buf[i%17] = pos - neg;          /* guaranteed non‑negative */
    }

    return buf[n%17];
}
