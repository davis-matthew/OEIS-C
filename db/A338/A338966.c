/*
 * A338966  Number of chiral pairs of colorings of the 120 dodecahedral facets
 *          of the 4-D 120-cell (or 120 vertices of the 4-D 600-cell) using
 *          subsets of a set of n colors.
 * https://oeis.org/A338966
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (960*n^4 + 1440*n^6 + 960*n^8 + 1200*n^10 + 336*n^12 +
 *            288*n^16 * - 1440*n^17 - 1440*n^19 + 40*n^20 + 400*n^22 -
 *            1200*n^23 * + 336*n^24 - 1200*n^27 + 60*n^30 - 1800*n^31 +
 *            288*n^32 * + 40*n^40 + 400*n^44 + n^60 - 60*n^61 + 450*n^62 * -
 *            60*n^75 + n^120) / 14400 * (Offset: 2)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n < 2 are irrelevant for the problem scope */
    if (n < 2) return (seq_t)0;

    /* base */
    seq_t n1 = (seq_t)n;

    /* successive powers of n */
    seq_t n2 = n1 * n1;          /* n^2   */
    seq_t n3 = n2 * n1;          /* n^3   */
    seq_t n4 = n3 * n1;          /* n^4   */
    seq_t n5 = n4 * n1;          /* n^5   */
    seq_t n6 = n5 * n1;          /* n^6   */
    seq_t n7 = n6 * n1;          /* n^7   */
    seq_t n8 = n7 * n1;          /* n^8   */
    seq_t n10 = n8 * n2;         /* n^10  */
    seq_t n12 = n10 * n2;        /* n^12  */
    seq_t n16 = n8 * n8;         /* n^16  */
    seq_t n17 = n16 * n1;        /* n^17  */
    seq_t n19 = n17 * n2;        /* n^19  */
    seq_t n20 = n16 * n4;        /* n^20  */
    seq_t n22 = n20 * n2;        /* n^22  */
    seq_t n23 = n22 * n1;        /* n^23  */
    seq_t n24 = n12 * n12;       /* n^24  */
    seq_t n27 = n24 * n3;        /* n^27  */
    seq_t n30 = n24 * n6;        /* n^30  */
    seq_t n31 = n30 * n1;        /* n^31  */
    seq_t n32 = n16 * n16;       /* n^32  */
    seq_t n40 = n32 * n8;        /* n^40  */
    seq_t n44 = n40 * n4;        /* n^44  */
    seq_t n60 = n40 * n20;       /* n^60  */
    seq_t n61 = n60 * n1;        /* n^61  */
    seq_t n62 = n60 * n2;        /* n^62  */
    seq_t n15 = n10 * n5;        /* n^15  */
    seq_t n75 = n60 * n15;       /* n^75  */
    seq_t n120 = n60 * n60;      /* n^120 */

    /* build numerator */
    seq_t num = 0;
    num += ((seq_t)960)  * n4;
    num += ((seq_t)1440) * n6;
    num += ((seq_t)960)  * n8;
    num += ((seq_t)1200) * n10;
    num += ((seq_t)336)  * n12;
    num += ((seq_t)288)  * n16;
    num -= ((seq_t)1440) * n17;
    num -= ((seq_t)1440) * n19;
    num += ((seq_t)40)   * n20;
    num += ((seq_t)400)  * n22;
    num -= ((seq_t)1200) * n23;
    num += ((seq_t)336)  * n24;
    num -= ((seq_t)1200) * n27;
    num += ((seq_t)60)   * n30;
    num -= ((seq_t)1800) * n31;
    num += ((seq_t)288)  * n32;
    num += ((seq_t)40)   * n40;
    num += ((seq_t)400)  * n44;
    num += ((seq_t)1)    * n60;
    num -= ((seq_t)60)   * n61;
    num += ((seq_t)450)  * n62;
    num -= ((seq_t)60)   * n75;
    num += ((seq_t)1)    * n120;

    /* divide by 14400 to obtain the term */
    return num / (seq_t)14400;
}
