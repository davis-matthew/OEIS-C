/*
 * A301718  Coordination sequence for node of type V2 in "kre" 2-D tiling (or
 *          net).
 * https://oeis.org/A301718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301718: Coordination sequence for node of type V2 in the
   "kre" 2‑D tiling.  It satisfies the recurrence

       a(0)=1,  a(1)=5,  a(2)=11,  a(3)=17,  a(4)=23,
       a(5)=28, a(6)=33,
       a(n)=a(n-1)+a(n-5)-a(n-6)   for n ≥ 7

   The generating function is
       (x^4+2*x^3+x^2+2*x+1)*(x+1)^2 / ((1-x)*(1-x^5)). */
seq_t a(idx_t n)
{
    static const seq_t base[7] = {1, 5, 11, 17, 23, 28, 33};

    if (n < 7)
        return base[n];

    /* buffer holds a_{n-6}, a_{n-5}, a_{n-4}, a_{n-3},
       a_{n-2}, a_{n-1} for the current n (starting with n = 7) */
    seq_t buf[6];
    buf[0] = base[1];   /* a1   */
    buf[1] = base[2];   /* a2   */
    buf[2] = base[3];   /* a3   */
    buf[3] = base[4];   /* a4   */
    buf[4] = base[5];   /* a5   */
    buf[5] = base[6];   /* a6   */

    idx_t i;
    seq_t next;
    for (i = 7; i <= n; ++i) {
        /* a_n = a_{n-1} + a_{n-5} - a_{n-6} */
        next = buf[5] + buf[1] - buf[0];

        /* shift buffer left by one */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];
}
