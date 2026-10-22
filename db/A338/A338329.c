/*
 * A338329  First differences of A326118.
 * https://oeis.org/A338329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A338329: First differences of A326118.
   a(0)=1, a(1)=1, a(2)=3.
   For n≥3, a(n) = (7+2*n-6*cos(nπ/2)+3*(-1)^n+6*sin(nπ/2))/4,
   which can be rewritten using n mod 4 as:
     n%4==0 → a(n) = 1 + n/2
     n%4==1 → a(n) = (n + 5)/2
     n%4==2 → a(n) = (n + 8)/2
     n%4==3 → a(n) = (n - 1)/2
*/
seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n <= 2) {
        if (n == 0 || n == 1) return (seq_t)1;
        return (seq_t)3;         /* n == 2 */
    }

    /* Compute using the simplified formulas above. */
    switch (n & 3u) {
        case 0u: /* n % 4 == 0, n is even */
            return (seq_t)1 + (seq_t)(n >> 1);
        case 1u: /* n % 4 == 1, n is odd */
            return (seq_t)((n + 5) >> 1);
        case 2u: /* n % 4 == 2, n is even */
            return (seq_t)((n + 8) >> 1);
        case 3u: /* n % 4 == 3, n is odd */
            return (seq_t)((n - 1) >> 1);
        default:
            return (seq_t)0;       /* unreachable */
    }
}
