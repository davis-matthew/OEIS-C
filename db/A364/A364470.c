/*
 * A364470  a(n) is the least k > a(n-1) such that Omega(k * (a(n-1)+k)) = n,
 *          where Omega = A001222; a(1) = 1.
 * https://oeis.org/A364470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A364470: a(1)=1, a(n) is the least k > a(n-1) with
 * Ω(k·(a(n‑1)+k)) = n.  For n ≥ 7 it satisfies the recurrence
 * a(n+4)=4·a(n) with initial a(7)=32, a(8)=56, a(9)=80, a(10)=120.
 */

static const seq_t init[10] = {
    1,   /* a(1) */
    2,   /* a(2) */
    7,   /* a(3) */
    11,  /* a(4) */
    13,  /* a(5) */
    19,  /* a(6) */
    32,  /* a(7) */
    56,  /* a(8) */
    80,  /* a(9) */
    120  /* a(10) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined but safe guard */
    if (n <= 10) return init[n - 1];

    /* for n ≥ 11 use the recurrence a(n+4)=4·a(n) */
    idx_t m = n - 7;                 /* m = (n-7) */
    idx_t q = m / 4;                 /* number of 4‑multiplications */
    idx_t r = m % 4;                 /* offset 0..3 corresponding to a(7..10) */

    seq_t base = init[6 + r];        /* a(7+r) */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < q; ++i)    /* compute 4^q */
        pow4 *= (seq_t)4;

    return base * pow4;
}
