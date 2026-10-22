/*
 * A320907  Row sums of A320906.
 * https://oeis.org/A320907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A320907: Row sums of A320906
   Recurrence: a(n) = 9*a(n‑1) - 31*a(n‑2) + 50*a(n‑3) - 36*a(n‑4) + 8*a(n‑5)
   Initial terms: a(0)=0, a(1)=1, a(2)=7, a(3)=33, a(4)=130
   The function computes a(n) for any n ≥ 0, using only seq_t arithmetic.
*/

seq_t a(idx_t n)
{
    /* Return the first five terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)33;
    if (n == 4) return (seq_t)130;

    /* Buffer holding the last five values a(i‑5) … a(i) */
    seq_t buf[5];
    buf[0] = (seq_t)0;   /* a(0) */
    buf[1] = (seq_t)1;   /* a(1) */
    buf[2] = (seq_t)7;   /* a(2) */
    buf[3] = (seq_t)33;  /* a(3) */
    buf[4] = (seq_t)130; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        /* Map: a(i‑1)=buf[4], a(i‑2)=buf[3], a(i‑3)=buf[2],
         * a(i‑4)=buf[1], a(i‑5)=buf[0]                           */
        seq_t a1 = buf[4];
        seq_t a2 = buf[3];
        seq_t a3 = buf[2];
        seq_t a4 = buf[1];
        seq_t a5 = buf[0];

        /* Compute positive and negative contributions separately
           to avoid underflow of unsigned seq_t types.             */
        seq_t pos = a1 * (seq_t)9;
        pos += a3 * (seq_t)50;
        pos += a5 * (seq_t)8;

        seq_t neg = a2 * (seq_t)31;
        neg += a4 * (seq_t)36;

        seq_t val = pos - neg;   /* a(i) = pos – neg */

        /* Shift the buffer for the next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = val;
    }

    return buf[4];
}
