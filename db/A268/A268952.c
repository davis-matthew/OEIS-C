/*
 * A268952  Number of length-n 0..4 arrays with no repeated value equal to the
 *          previous repeated value, with new values introduced in sequential
 *          order.
 * https://oeis.org/A268952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A268952
   Sequence of length‑n arrays over {0..4} with
   new values introduced in sequential order and no
   repeated value adjacent to an earlier repetition.

   Computed by the linear recurrence (for n>11):
     a(1)=1, a(2)=2, a(3)=4, a(4)=12, a(5)=40,
     a(6)=153, a(7)=634, a(8)=2785, a(9)=12634,
     a(10)=58409, a(11)=272738
     a(n)=11*a(n-1)-31*a(n-2)-41*a(n-3)+268*a(n-4)
          -90*a(n-5)-692*a(n-6)+512*a(n-7)+576*a(n-8)-512*a(n-9)
*/
seq_t a(idx_t n)
{
    /* first 11 terms */
    static const seq_t init[11] = {
        1ULL, 2ULL, 4ULL, 12ULL, 40ULL, 153ULL,
        634ULL, 2785ULL, 12634ULL, 58409ULL, 272738ULL
    };
    if (n <= 11)
        return init[n - 1];

    /* buffer holds previous 9 values:
       buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[8] = a(k-9)   */
    seq_t buf[9];
    buf[0] = init[10]; /* a(11) */
    buf[1] = init[9];  /* a(10) */
    buf[2] = init[8];  /* a(9)  */
    buf[3] = init[7];  /* a(8)  */
    buf[4] = init[6];  /* a(7)  */
    buf[5] = init[5];  /* a(6)  */
    buf[6] = init[4];  /* a(5)  */
    buf[7] = init[3];  /* a(4)  */
    buf[8] = init[2];  /* a(3)  */

    seq_t cur = 0;
    for (idx_t i = 12; i <= n; ++i) {
        /* compute next term from the recurrence */
        seq_t pos = 11 * buf[0] + 268 * buf[3] + 512 * buf[6] + 576 * buf[7];
        seq_t neg = 31 * buf[1] + 41 * buf[2] + 90 * buf[4]
                    + 692 * buf[5] + 512 * buf[8];
        cur = pos - neg;   /* safe: pos >= neg for all n >= 12 */

        /* shift buffer: new previous values */
        for (idx_t j = 8; j > 0; --j)
            buf[j] = buf[j - 1];
        buf[0] = cur;
    }
    return cur;
}
