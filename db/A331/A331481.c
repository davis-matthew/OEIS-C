/*
 * A331481  Position of the first letter of n-th (in English) in the alphabet.
 * https://oeis.org/A331481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = first letter of the n‑th English ordinal word (as
 *            alphabet position)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ordinal names 1–19 (F,S,T etc.) */
    static const seq_t ordinal_map[20] = {
        0,  6, 19, 20, 6, 6, 19, 19, 5, 14,
        20, 5, 20, 20, 6, 6, 19, 19, 5, 14
    };

    /* first letter of the cardinal name for 1–19 */
    static const seq_t cardinal_first[20] = {
        0,  /* unused */
        15, /* One   */
        20, /* Two   */
        20, /* Three */
        6,  /* Four  */
        6,  /* Five  */
        19, /* Six   */
        19, /* Seven */
        5,  /* Eight */
        14, /* Nine  */
        20, /* Ten   */
        5,  /* Eleven*/
        20, /* Twelve*/
        20, /* Thirteen*/
        6,  /* Fourteen*/
        6,  /* Fifteen */
        19, /* Sixteen */
        19, /* Seventeen*/
        5,  /* Eighteen*/
        14  /* Nineteen*/
    };

    /* first letter of the tens word 20,30,…,90 */
    static const seq_t tens_map[10] = { 0, 0, 20, 20, 6, 6, 19, 19, 5, 14 };

    if (n == 0) return 26;           /* "Zero" → Z = 26 */

    if (n <= 19) return ordinal_map[(int)n];

    /* For n ≥ 20, the first letter of its ordinal word equals the first
       letter of the cardinal representation of the most significant
       thousand-group.  We reduce the number until it is < 1000. */
    uint64_t m = (uint64_t)n;
    while (m >= 1000) m /= 1000;

    if (m < 20) {
        return cardinal_first[(int)m];
    } else if (m < 100) {
        return tens_map[(int)(m / 10)];
    } else {
        /* m is 100–999 – first word is the hundreds digit */
        return cardinal_first[(int)(m / 100)];
    }
}
