/*
 * A245180  A160239(n)/8.
 * https://oeis.org/A245180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 8^(r-1) * Π R(i), where r = number of runs of 1's in the
 *            binary expansion of n, i = length of each run, R(i) =
 *            (5*4^(i-1)+(-2)^(i-1))/6 = (5*4^(i-1) ± 2^(i-1))/6 (± according
 *            to parity)
 * Range:     u8 n <= 14, u16 n <= 766, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 91 terms through n = 9924 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t run_value(idx_t len)
{
    seq_t pow4 = (seq_t)1;
    seq_t pow2 = (seq_t)1;
    for (idx_t k = 1; k < len; ++k) {
        pow4 *= (seq_t)4;
        pow2 *= (seq_t)2;
    }
    seq_t total;
    if ((len - 1) & 1) {           /* odd exponent of -2 */
        total = (seq_t)5 * pow4 - pow2;
    } else {                       /* even exponent of -2  -> +2^(len-1) */
        total = (seq_t)5 * pow4 + pow2;
    }
    return total / (seq_t)6;
}

seq_t a(idx_t n)
{
    idx_t tmp   = n;
    idx_t len   = 0;        /* length of current run of 1's */
    idx_t runs  = 0;        /* number of runs found */
    seq_t prod  = (seq_t)1; /* product of R(i) values */

    while (tmp > 0) {
        if (tmp & 1) {                /* still inside a run of 1's */
            ++len;
        } else {                      /* run terminated */
            if (len > 0) {
                ++runs;
                prod *= run_value(len);
                len = 0;
            }
        }
        tmp >>= 1;
    }
    if (len > 0) {                     /* last run reaches the most significant bit */
        ++runs;
        prod *= run_value(len);
    }

    seq_t pow8 = (seq_t)1;
    for (idx_t i = 1; i < runs; ++i) { /* multiply by 8^(runs-1) */
        pow8 *= (seq_t)8;
    }

    return prod * pow8;
}
