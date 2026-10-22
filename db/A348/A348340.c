/*
 * A348340  For n >= 1; x = n, then iterate x --> x + h(x) until h(x + h(x))
 *          >= h(x). a(n) gives the number of iteration steps where h(i) is
 *          A000120(i).
 * https://oeis.org/A348340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of x → x + popcount(x) until
 *            popcount decreases, starting from n
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* popcount for seq_t (works for built‑in integer types and arbitrary‑precision int types that support & and -) */
static inline idx_t popcnt_seq(seq_t x)
{
    idx_t c = 0;
    while (x) {
        x &= x - 1;   /* clear lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;                /* current value */
    idx_t h_cur = popcnt_seq(x);        /* popcount of current value */
    seq_t steps = 0;                    /* number of steps taken */

    while (1) {
        x = x + (seq_t)h_cur;         /* transition to next value */
        ++steps;                      /* count this step */

        idx_t h_next = popcnt_seq(x);  /* popcount of new value */
        if (h_next < h_cur) {         /* terminate when popcount decreases */
            break;
        }
        h_cur = h_next;                /* continue with new popcount */
    }
    return steps;
}
