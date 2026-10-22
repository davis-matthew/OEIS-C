/*
 * A311936  Coordination sequence Gal.6.300.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311936
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

/*  A311936:  Coordination sequence Gal.6.300.2
    Formula: a(n) =  2*a(n-10) - a(n-20)   for n > 23
             a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=17,
             a(5)=22, a(6)=26, a(7)=31, a(8)=36, a(9)=40,
             a(10)=45, a(11)=48, a(12)=54, a(13)=57,
             a(14)=63, a(15)=62, a(16)=72, a(17)=74,
             a(18)=82, a(19)=83, a(20)=91, a(21)=91,
             a(22)=100, a(23)=100
    The function computes a(n) for any n≥0, memoising results for
    efficiency. */

#include <stdlib.h>

static const seq_t _a_init[24] = {
    1, 4, 8, 13, 17, 22, 26, 31, 36, 40,
    45, 48, 54, 57, 63, 62, 72, 74, 82, 83,
    91, 91, 100, 100
};

/* External linkage as required */
seq_t a(idx_t n)
{
    /* If below known terms, return directly */
    if (n < 24)
        return _a_init[n];

    /* static state for memoisation */
    static seq_t *buf          = NULL;   /* array of computed values   */
    static idx_t  buf_len      = 0;      /* allocated size in elements */
    static idx_t  computed_upto = 23;    /* largest index with a value  */

    /* initialise buffer on first use */
    if (!buf) {
        buf_len = (n + 1 > 24) ? n + 1 : 24;
        buf = (seq_t *)malloc(buf_len * sizeof(seq_t));
        if (!buf) {                 /* out of memory; give up */
            return 0;
        }
        for (idx_t i = 0; i < 24; ++i)
            buf[i] = _a_init[i];
        for (idx_t i = 24; i < buf_len; ++i)
            buf[i] = 0;
    }

    /* enlarge buffer if needed */
    if (n + 1 > buf_len) {
        idx_t old_len = buf_len;
        buf_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, buf_len * sizeof(seq_t));
        if (!tmp) {                 /* out of memory */
            return 0;
        }
        buf = tmp;
        for (idx_t i = old_len; i < buf_len; ++i)
            buf[i] = 0;
    }

    /* compute values up to n using the recurrence */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        buf[i] = (seq_t)(2 * buf[i - 10] - buf[i - 20]);
    }
    computed_upto = n;

    return buf[n];
}
