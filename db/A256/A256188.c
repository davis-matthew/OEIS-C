/*
 * A256188  In positive integers: replace k*(k+1)/2 with the first k numbers.
 * https://oeis.org/A256188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 486, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A256188: replace triangular numbers k(k+1)/2 with the first k numbers.
   The function builds the modified sequence on demand, iterating over the
   original positive integers and expanding blocks for each triangular
   term.  It works for any order of calls and for any integer width of
   seq_t and idx_t.                                        */
seq_t a(idx_t n)
{
    /* index 1 is the first term of the sequence; requests for n<=0
       are not defined by the OEIS offset, but we return 0 safely. */
    if (n == 0) return 0;

    idx_t pos = 1;          /* current output position */
    idx_t m   = 1;          /* current original integer */
    idx_t triK = 1;         /* current triangular index k */
    __uint128_t nextTri = 1;/* next triangular value k(k+1)/2 */

    while (1) {
        if ( ( __uint128_t)m == nextTri ) {          /* m is triangular */
            idx_t block_start = pos;
            idx_t block_end   = pos + triK - 1;

            if (n >= block_start && n <= block_end) {
                return (seq_t)(n - block_start + 1); /* value in the block */
            }

            pos = block_end + 1;                     /* skip the block */

            triK++;                                 /* prepare next k    */
            nextTri = ( (__uint128_t)triK * (triK + 1) ) / 2;
        } else {                                    /* normal integer    */
            if (n == pos) {
                return (seq_t)m;                     /* value is m itself */
            }
            pos++;
        }
        m++;                                        /* advance to next map
                                                       number m */
    }
}
