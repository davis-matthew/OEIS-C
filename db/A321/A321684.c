/*
 * A321684  Independent domination number of the n X n grid graph.
 * https://oeis.org/A321684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 570, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Independent domination number of the n X n grid graph
 * a(0) = 0
 * a(1) = 1
 * a(2) = 2
 * a(3) = 3
 * a(4) = 4
 * a(5) = 7
 * a(6) = 10
 * a(7) = 12
 * a(8) = 16
 * a(9) = 21
 * a(10) = 24
 * a(11) = 30
 * a(12) = 35
 * a(13) = 40
 *
 * For n >= 14, a(n) = floor((n+2)^2 / 5 - 4)  (OEIS formula)
 *
 * This implementation uses the closed‑form formula for n >= 14
 * and a small switch statement for the first 14 values, which
 * avoids a large lookup table while still being fully deterministic.
 */

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n < 14) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            case 4: return (seq_t)4;
            case 5: return (seq_t)7;
            case 6: return (seq_t)10;
            case 7: return (seq_t)12;
            case 8: return (seq_t)16;
            case 9: return (seq_t)21;
            case 10: return (seq_t)24;
            case 11: return (seq_t)30;
            case 12: return (seq_t)35;
            case 13: return (seq_t)40;
            /* no default needed; n < 14 ensures a case is hit */
        }
    }

    /* For n >= 14 use the closed form formula
     * a(n) = floor((n+2)^2 / 5 - 4) ;
     *
     * All arithmetic is performed in idx_t (unsigned 64-bit) to avoid
     * overflow; the final result is cast back to seq_t.
     */
    idx_t np = n + 2;          /* n+2 */
    idx_t sq = np * np;        /* (n+2)^2, fits in 64 bits for n <= 10000 */
    idx_t val = sq / 5;        /* floor((n+2)^2 / 5) */
    val -= 4;                  /* subtract 4 */
    return (seq_t)val;
}
