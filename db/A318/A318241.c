/*
 * A318241  Column 3 of array in A318240.
 * https://oeis.org/A318241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=0; a(6k+1)=a(6k+2)=a(6k+3)=3k+1; a(6k)=a(6k-1)=a(6k-2)=3k
 *            or 3k+1 (a(6)=4)
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;

    idx_t rem = n % (idx_t)6;
    idx_t k;
    seq_t val;

    switch (rem)
    {
        case 1:
        case 2:
        case 3:
            k = (n - (idx_t)1) / (idx_t)6;
            val = ( (seq_t)k * (seq_t)3 ) + (seq_t)1;
            break;

        case 4:
            k = (n - (idx_t)4) / (idx_t)6;
            val = ( ((seq_t)(k + (idx_t)1)) * (seq_t)3 );
            break;

        case 5:
            k = (n + (idx_t)1) / (idx_t)6;
            val = (seq_t)k * (seq_t)3;
            break;

        case 0:
            k = n / (idx_t)6;
            if (k == (idx_t)1)
                val = ( ((seq_t)k) * (seq_t)3 ) + (seq_t)1;
            else
                val = ((seq_t)k) * (seq_t)3;
            break;

        default:   /* unreachable */
            val = (seq_t)0;
    }

    return val;
}
