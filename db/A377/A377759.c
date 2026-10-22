/*
 * A377759  Number of edge cuts in the n-double cone graph.
 * https://oeis.org/A377759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n - A158525(n+1)^2 * Recurrence: a(n) =
 *            35*a(n-1)-408*a(n-2)+1898*a(n-3)-3980*a(n-4)+3880*a(n-5)-1680*a(n-6)+256*a(n-7)
 *            * Initial values:
 *            a(0)=1,a(1)=12,a(2)=156,a(3)=2652,a(4)=47580,a(5)=835132,a(6)=14274492
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the vault of known initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)156;
    if (n == 3) return (seq_t)2652;
    if (n == 4) return (seq_t)47580;
    if (n == 5) return (seq_t)835132;
    if (n == 6) return (seq_t)14274492;

    /* last[0] == a(n-1), last[6] == a(n-7) */
    seq_t last[7] = {
        (seq_t)14274492,    /* a6  */
        (seq_t)835132,      /* a5  */
        (seq_t)47580,       /* a4  */
        (seq_t)2652,        /* a3  */
        (seq_t)156,         /* a2  */
        (seq_t)12,          /* a1  */
        (seq_t)1            /* a0  */
    };

    /* Compute successive values up to n */
    seq_t cur;
    for (idx_t i = 7; i <= n; ++i) {
        cur =  (seq_t)35  * last[0]
             - (seq_t)408 * last[1]
             + (seq_t)1898* last[2]
             - (seq_t)3980* last[3]
             + (seq_t)3880* last[4]
             - (seq_t)1680* last[5]
             + (seq_t)256 * last[6];

        /* Shift the window: last[1] becomes last[0], etc. */
        for (int k = 5; k >= 0; --k) {
            last[k + 1] = last[k];
        }
        last[0] = cur;
    }

    return cur;
}
