/*
 * A262862  Decimal representation of the n-th iteration of the "Rule 147"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A262862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((16 + 6*(n mod 2))*4^n / 15)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5, a(2)=17, a(3)=93           */
/* Recurrence: a(n) = 17*a(n-2) - 16*a(n-4) for n > 3       */
#include <stdint.h>

static inline seq_t mul_const(seq_t a, unsigned int c)
{
#ifdef __SIZEOF_INT128__
    if (sizeof(seq_t) <= sizeof(unsigned long long))
        return (seq_t)((unsigned __int128)a * c);
#endif
    return a * (seq_t)c;
}

seq_t a(idx_t n)
{
    /* Initial terms for n = 0..3 */
    static const seq_t init[4] = {1, 5, 17, 93};

    if (n < 4)
        return init[n];

    /* Buffer to keep the last four computed terms */
    seq_t buf[4];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];

    for (idx_t i = 4; i <= n; ++i) {
        /* indices modulo 4 for a(i-2) and a(i-4) */
        idx_t i2 = (i - 2) & 3;
        idx_t i4 = (i - 4) & 3;

        seq_t t1 = mul_const(buf[i2], 17);  /* 17 * a(i-2) */
        seq_t t2 = mul_const(buf[i4], 16);  /* 16 * a(i-4) */
        seq_t cur = t1 - t2;                /* a(i) = 17*a(i-2) - 16*a(i-4) */

        buf[i & 3] = cur;
    }

    return buf[n & 3];
}
