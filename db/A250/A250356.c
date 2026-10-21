/*
 * A250356  Number of length 7 arrays x(i), i=1..7 with x(i) in i..i+n and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 - 14*n^5 + 105*n^4 + 105*n^3 - 532*n^2 +
 *            1252*n - 744, for n>4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (hard‑coded first four terms) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 4) {
        switch ((uint64_t)n) {
            case 1: return (seq_t)128;
            case 2: return (seq_t)1791;
            case 3: return (seq_t)12620;
            case 4: return (seq_t)59406;
        }
    }

    /* Compute using 128‑bit arithmetic to avoid intermediate overflow */
    unsigned __int128 nn  = n;
    unsigned __int128 n2  = nn * nn;
    unsigned __int128 n3  = n2 * nn;
    unsigned __int128 n4  = n3 * nn;
    unsigned __int128 n5  = n4 * nn;
    unsigned __int128 n6  = n5 * nn;
    unsigned __int128 n7  = n6 * nn;
    unsigned __int128 val = n7;

    val += 7  * n6;
    val -= 14 * n5;
    val += 105 * n4;
    val += 105 * n3;
    val -= 532 * n2;
    val += 1252 * nn;
    val -= 744;

    return (seq_t)val;
}
