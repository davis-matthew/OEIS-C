/*
 * A359508  a(n) = log_2(A359507(n) - 1).
 * https://oeis.org/A359508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = log₂(A359507(n)-1). Computed via recurrence: a(1) = 0,
 *            a(2) = 1, a(3) = 1; for k>0: a(4k) = 1, a(4k+1) = 2, a(4k+2) =
 *            1, a(4k+3) = a(k) + 2
 * Range:     u8 n <= 9519, u16 n <= 9519, u32 n <= 9519, u64 n <= 9519,
 *            u128 n <= 9519, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle possible index 0 which is outside the offset; return 0. */
    if (n == 0)
        return 0;

    /* Base cases for n <= 3. */
    if (n <= 3) {
        if (n == 1) return 0;
        if (n == 2) return 1;
        /* n == 3 */
        return 1;
    }

    /* Accumulate +2 for every time we encounter a 4k+3 pattern. */
    seq_t add = 0;

    while (n >= 4) {
        /* Only the 4k+3 case adds to the result and reduces n. */
        if ((n & 3) == 3) {          /* n % 4 == 3  */
            add += (seq_t)2;
            n >>= 2;                 /* n /= 4; shift right by 2 bits */
        } else {
            break;                   /* other residues handled below */
        }
    }

    /* Now n is < 4 or n % 4 != 3 but n >= 4; fetch the base value. */
    seq_t base;
    if (n == 1) {
        base = 0;
    } else if (n == 2) {
        base = 1;
    } else if (n == 3) {
        base = 1;
    } else {
        /* n >= 4 and n % 4 is 0, 1, or 2 */
        switch ((int)(n & 3)) {
            case 0: /* 4k */
                base = 1;
                break;
            case 1: /* 4k+1 */
                base = 2;
                break;
            case 2: /* 4k+2 */
                base = 1;
                break;
            default:
                base = 0; /* unreachable */
        }
    }

    return add + base;
}
