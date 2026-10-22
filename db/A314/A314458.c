/*
 * A314458  Coordination sequence Gal.3.40.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=0; 5 if n=1; 8 if n=2. For n≥3, a(n) = 13 +
 *            14*floor((n-3)/3) + inc[(n-3)%3], where inc[0]=0, inc[1]=7,
 *            inc[2]=10
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence a(n)=a(n-1)+[7,3,4] cycles for n≥3. */
seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;

    /* n ≥ 3 */
    idx_t k = n - 3;          /* shift so cycle starts at k=0 */
    idx_t q = k / 3;          /* number of full 3‑step blocks */
    idx_t r = k % 3;          /* position inside the block */

    seq_t res = (seq_t)13;              /* a(3) */
    res += (seq_t)14 * (seq_t)q;        /* 14 per full block */

    /* add partial increment depending on remainder */
    if (r == 1) {
        res += (seq_t)7;   /* first step of the cycle: +7 */
    } else if (r == 2) {
        res += (seq_t)10;  /* +7 + 3 = 10 for two steps */
    } else {
        /* r == 0: nothing extra */
    }
    return res;
}
