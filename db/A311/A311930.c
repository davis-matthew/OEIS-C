/*
 * A311930  Coordination sequence Gal.5.104.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Coordination sequence Gal.5.104.3 (OEIS A311930)
 *  a(0) = 1
 *  a(2k) = 9k - 1   (k >= 1, i.e. for even n >= 2 : a(n) = 9n/2 - 1)
 *  a(2k+1) = first four odd terms are fixed:
 *      a(1)=4, a(3)=13, a(5)=22, a(7)=31
 *  for odd n >= 9:
 *      let m = (n - 9) / 2   (number of 2‑step increments beyond n=9)
 *      a(9)   = 39
 *      a(9+2i) = 39 + S(i)   where S(i) is the cumulative sum of a fixed
 *                               difference pattern 8,10,8,8,8,10 repeated
 *                               after the first two steps.
 *
 *  The pattern of differences for odd indices >=9 is:
 *      +8, +10, +8, +8, +8, +10, +8, +8, +8, +10, +8, ...
 *  This yields a closed form using 8‑block cycles of four terms
 *  (sum 34).  The implementation below follows that logic.
 *  The function is fully deterministic and works for any idx_t up to
 *  uint64_t with seq_t of any unsigned width.
 */

seq_t a(idx_t n) {
    /* Handle the offset and the first few special cases */
    if (n == 0)
        return (seq_t)1;

    /* Even indices (n >= 2) */
    if ((n & 1) == 0) {
        /* a(n) = 9*n/2 - 1 for even n >= 2 */
        return (seq_t)(((seq_t)n * (seq_t)9) / (seq_t)2) - (seq_t)1;
    }

    /* Odd indices ----------------------------------------------------- */
    switch (n) {
        case 1: return (seq_t)4;
        case 3: return (seq_t)13;
        case 5: return (seq_t)22;
        case 7: return (seq_t)31;
        /* n == 9 handled below via m==0 */
    }

    /* n >= 9 and odd */
    idx_t m = (n - 9) >> 1;          /* m = (n-9)/2  */
    seq_t sum;

    if (m == 0)        /* n == 9 */
        sum = (seq_t)0;
    else if (m == 1)   /* n == 11 */
        sum = (seq_t)8;
    else if (m == 2)   /* n == 13 */
        sum = (seq_t)18;   /* 8 + 10 */
    else {             /* m >= 3, use cycle of 8,8,8,10 (sum 34) */
        seq_t base = (seq_t)18;                /* sum for first two steps */
        idx_t r = m - 2;                       /* remaining steps */
        seq_t c = r / 4;                       /* full cycles of length 4 */
        idx_t t = r % 4;                       /* remainder in current cycle */
        static const seq_t prefix[4] = {0, 8, 16, 24};
        sum = base + c * (seq_t)34 + prefix[t];
    }

    return (seq_t)39 + sum;  /* a(9) = 39 */
}
