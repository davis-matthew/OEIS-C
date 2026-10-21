/*
 * A243648  Number of ways five L-tiles can be placed on an n X n square.
 * https://oeis.org/A243648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 -10*n^9 -25*n^8 +520*n^7 -435*n^6 -9982*n^5
 *            +19925*n^4 +82740*n^3 -215906*n^2 -244868*n +728760) / 120 for n
 *            >= 5, a(n) = 0 for n < 5
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 16, u64 n <= 137, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit intermediate when seq_t is small */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 un = (unsigned __int128)n;
        unsigned __int128 n2 = un * un;
        unsigned __int128 n3 = n2 * un;
        unsigned __int128 n4 = n3 * un;
        unsigned __int128 n5 = n4 * un;
        unsigned __int128 n6 = n5 * un;
        unsigned __int128 n7 = n6 * un;
        unsigned __int128 n8 = n7 * un;
        unsigned __int128 n9 = n8 * un;
        unsigned __int128 n10 = n9 * un;

        unsigned __int128 numer = n10
             - (unsigned __int128)10 * n9
             - (unsigned __int128)25 * n8
             + (unsigned __int128)520 * n7
             - (unsigned __int128)435 * n6
             - (unsigned __int128)9982 * n5
             + (unsigned __int128)19925 * n4
             + (unsigned __int128)82740 * n3
             - (unsigned __int128)215906 * n2
             - (unsigned __int128)244868 * un
             + (unsigned __int128)728760;

        unsigned __int128 res128 = numer / (unsigned __int128)120;
        return (seq_t)res128;
    }
#endif

    /* 64‑bit or arbitrary‑precision implementation */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2  * nn;
    seq_t n4  = n3  * nn;
    seq_t n5  = n4  * nn;
    seq_t n6  = n5  * nn;
    seq_t n7  = n6  * nn;
    seq_t n8  = n7  * nn;
    seq_t n9  = n8  * nn;
    seq_t n10 = n9  * nn;

    seq_t numer = n10
        - (seq_t)10 * n9
        - (seq_t)25 * n8
        + (seq_t)520 * n7
        - (seq_t)435 * n6
        - (seq_t)9982 * n5
        + (seq_t)19925 * n4
        + (seq_t)82740 * n3
        - (seq_t)215906 * n2
        - (seq_t)244868 * nn
        + (seq_t)728760;

    return numer / (seq_t)120;
}
