/*
 * A358249  Number of n-regular, N_0-weighted pseudographs on 2 vertices with
 *          total edge weight 9, up to isomorphism.
 * https://oeis.org/A358249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 776 for n >= 18. Initial values: a(1)=1,
 *            a(2)=10, a(3)=42, a(4)=123, a(5)=259, a(6)=469, a(7)=721,
 *            a(8)=1034, a(9)=1359, a(10)=1726, a(11)=2082, a(12)=2472,
 *            a(13)=2840, a(14)=3239, a(15)=3611, a(16)=4013, a(17)=4386
 * Range:     u8 n <= 4, u16 n <= 174, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed values for 1 <= n <= 17 */
    static const seq_t base[18] = {
        0ULL,   /* dummy 0 index */
        1ULL,   /* a(1) */
        10ULL,  /* a(2) */
        42ULL,  /* a(3) */
        123ULL, /* a(4) */
        259ULL, /* a(5) */
        469ULL, /* a(6) */
        721ULL, /* a(7) */
        1034ULL,/* a(8) */
        1359ULL,/* a(9) */
        1726ULL,/* a(10) */
        2082ULL,/* a(11) */
        2472ULL,/* a(12) */
        2840ULL,/* a(13) */
        3239ULL,/* a(14) */
        3611ULL,/* a(15) */
        4013ULL,/* a(16) */
        4386ULL /* a(17) */
    };
    const seq_t A18 = 4789ULL;
    const seq_t A19 = 5162ULL;
    const seq_t STEP = 776ULL;

    if (n <= 17)
        return base[n];

    if ((n & 1ULL) == 0ULL) {          /* even n >= 18 */
        seq_t k = (seq_t)((n - (idx_t)18) / 2); /* number of steps from 18 */
        return A18 + k * STEP;
    } else {                            /* odd n >= 19 */
        seq_t k = (seq_t)((n - (idx_t)19) / 2); /* number of steps from 19 */
        return A19 + k * STEP;
    }
}
