/*
 * A295515  The Euclid tree, read across levels.
 * https://oeis.org/A295515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fusc(floor(n/2)) (a(1) = 0)
 * Range:     u8 n <= 8190, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* fusc(n) is Stern's diatomic sequence:                      */
/*   fusc(0)=0,  fusc(1)=1,  fusc(2k)=fusc(k),                */
/*   fusc(2k+1)=fusc(k)+fusc(k+1).                            */
/* The function below implements fusc iteratively using only     */
/* integer operations, fitting the required type restrictions.  */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;          /* offset 1: first term is 0 */

    /* floor(n/2) is the argument for the fusc function */
    idx_t m = n >> 1;       /* equivalent to n/2 but faster */

    seq_t a_val = 0;        /* fusc(0) */
    seq_t b_val = 1;        /* auxiliary value */

    /* Iterate over bits of m from least‑significant to most‑significant */
    for (idx_t x = m; x != 0; x >>= 1) {
        if (x & 1) {               /* odd bit  ->  a = a + b   */
            a_val = a_val + b_val;
        } else {                    /* even bit ->  b = a + b   */
            b_val = a_val + b_val;
        }
    }

    return a_val;           /* fusc(m) */
}
