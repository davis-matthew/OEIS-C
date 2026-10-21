/*
 * A255299  A255298(2^n-1).
 * https://oeis.org/A255299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A255299:
   a(0)=1, a(1)=4, a(2)=16, a(3)=58, a(4)=204, a(5)=714,
   a(6)=2492, a(7)=8682.
   For n≥8:
   a(n)=6*a(n‑1)-10*a(n‑2)+4*a(n‑3)+3*a(n‑4)-12*a(n‑5)
        +20*a(n‑6)-10*a(n‑7)+4*a(n‑8) */
seq_t a(idx_t n)
{
    /* initial values a(0)…a(7) */
    seq_t val[8];
    val[0] = 1;  val[1] = 4;  val[2] = 16;  val[3] = 58;
    val[4] = 204; val[5] = 714; val[6] = 2492; val[7] = 8682;

    if (n < 8)
        return val[n];

    /* pos points to a(n-1) in the circular buffer */
    int pos = 7;
    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t t = 0;
        t += 6 * val[pos];
        t -= 10 * val[(pos + 7) % 8];
        t += 4 * val[(pos + 6) % 8];
        t += 3 * val[(pos + 5) % 8];
        t -= 12 * val[(pos + 4) % 8];
        t += 20 * val[(pos + 3) % 8];
        t -= 10 * val[(pos + 2) % 8];
        t += 4 * val[(pos + 1) % 8];

        pos = (pos + 1) % 8;
        val[pos] = t;
    }
    return val[pos];
}
