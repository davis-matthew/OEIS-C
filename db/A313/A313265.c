/*
 * A313265  Coordination sequence Gal.6.475.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=a(n-1)-a(n-2)+a(n-3)-a(n-4)+2*a(n-5)-2*a(n-6)+2*a(n-7) *
 *            -a(n-8)+a(n-9)-a(n-10)+a(n-11)-a(n-12) for n>14. * Initial terms
 *            (n=0..14): * 1,4,9,15,20,25,32,37,40,45,51,56,61,66,72
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[15] = {
        1, 4, 9, 15, 20, 25, 32, 37, 40, 45, 51, 56, 61, 66, 72
    };

    if (n < 15) return init[n];

    seq_t buf[15];
    for (int i = 0; i < 15; ++i) buf[i] = init[i];

    for (idx_t i = 15; i <= n; ++i) {
        /* indices of previous 13 terms */
        seq_t val =
            buf[(i-1) % 15] - buf[(i-2) % 15] + buf[(i-3) % 15]
          - buf[(i-4) % 15] + 2 * buf[(i-5) % 15]
          - 2 * buf[(i-6) % 15] + 2 * buf[(i-7) % 15]
          - buf[(i-8) % 15] + buf[(i-9) % 15]
          - buf[(i-10) % 15] + buf[(i-11) % 15]
          - buf[(i-12) % 15];

        buf[i % 15] = val;
    }

    return buf[n % 15];
}
