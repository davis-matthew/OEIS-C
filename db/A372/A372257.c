/*
 * A372257  An infinite sequence over {0,1,2,3} without palindromes of length
 *          more than 1.
 * https://oeis.org/A372257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 4) XOR A014707(floor(n/2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is implemented by traversing the morphism 0->01, 1->23, 2->45, 3->23,
   4->01, 5->67, 6->45, 7->67 and then coding with 0->0, 1->1, 2->2, 3->3,
   4->1, 5->0, 6->3, 7->2. */
seq_t a(idx_t n)
{
    /* left and right child at each expansion step for the 8-symbol morphism */
    static const unsigned char left_child[8]  = {0,2,4,2,0,6,4,6};
    static const unsigned char right_child[8] = {1,3,5,3,1,7,5,7};

    /* coding from 0..7 to the final 0..3 alphabet */
    static const unsigned char encode[8] = {0,1,2,3,1,0,3,2};

    unsigned char d = 0;                 /* start symbol (0) */

    /* process all 64 bits of n; leading zeros perform left_child on 0,
       which keeps d at 0 so the result is correct for n == 0. */
    for (int i = 63; i >= 0; --i) {
        if ((n >> i) & 1ULL)
            d = right_child[d];
        else
            d = left_child[d];
    }

    return (seq_t)encode[d];
}
