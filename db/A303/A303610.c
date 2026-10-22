/*
 * A303610  Circle aliasing numbers with 1/n size steps.
 * https://oeis.org/A303610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303610: Circle aliasing numbers with 1/n size steps.
   For a given n ≥ 1 the sequence is defined by a greedy walk that
   starts at (‑1,0) and, at each of the 2 n steps of length 1/n,
   moves either up (1) or right (0) so as to minimize the absolute
   difference |x² + y² − 1|.  Ties are broken in favor of an up move.
   The value returned is the decimal integer whose decimal digits
   are the sequence of 0/1 moves (e.g. 110100 for n = 3).  */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 for safety */

    /* Integer counts of right and up steps taken so far. */
    idx_t xsteps = 0;
    idx_t ysteps = 0;

    /* dev = (dx² + u²) − n², where dx = xsteps − n, u = ysteps.
       The value is kept in a 64‑bit signed integer; it stays
       comfortably within that range for the required n. */
    long long dev = 0;             /* init: dx = –n, u = 0 → dev = 0 */

    seq_t res = 0;                 /* the decimal number being built */

    for (idx_t step = 0; step < 2 * n; ++step)
    {
        long long dev_up    = dev + 2LL * (long long)ysteps + 1;
        long long dev_right = dev
                              + 2LL * ((long long)xsteps - (long long)n) + 1;

        long long abs_up    = dev_up  >= 0 ? dev_up  : -dev_up;
        long long abs_right = dev_right>= 0 ? dev_right : -dev_right;

        int bit;
        if (abs_up <= abs_right)          /* choose up on tie */
        {
            bit = 1;
            ++ysteps;
            dev = dev_up;
        }
        else
        {
            bit = 0;
            ++xsteps;
            dev = dev_right;
        }

        /* Append the new digit as a decimal digit. */
        res = res * 10 + (seq_t)bit;
    }

    return res;
}
