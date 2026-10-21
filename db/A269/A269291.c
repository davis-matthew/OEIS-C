/*
 * A269291  Number of 3 X n 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          no more than once.
 * https://oeis.org/A269291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=64, a(2)=2592, a(3)=62748, a(4)=1363572,
   a(5)=27788292, a(6)=544118148, a(7)=10363900428
   a(n) = 36*a(n-1) - 378*a(n-2) + 972*a(n-3) - 729*a(n-4)   (n>7) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n <= 7) {
        static const seq_t base[8] = {
            0ULL,
            64ULL,
            2592ULL,
            62748ULL,
            1363572ULL,
            27788292ULL,
            544118148ULL,
            10363900428ULL
        };
        return base[n];
    }
    seq_t a_nm4 = 1363572ULL;   /* a(4) */
    seq_t a_nm3 = 27788292ULL;  /* a(5) */
    seq_t a_nm2 = 544118148ULL; /* a(6) */
    seq_t a_nm1 = 10363900428ULL; /* a(7) */
    seq_t cur = 0;
    for (idx_t i = 8; i <= n; ++i) {
        cur = 36 * a_nm1;
        cur -= 378 * a_nm2;
        cur += 972 * a_nm3;
        cur -= 729 * a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
