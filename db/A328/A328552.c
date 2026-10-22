/*
 * A328552  a(n) is the Severi degree for curves of degree n and cogenus 5.
 * https://oeis.org/A328552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (81/40)*n^10 - (81/4)*n^9 - (27/8)*n^8 + (2349/4)*n^7 -
 *            1044*n^6 - (127071/20)*n^5 + (128859/8)*n^4 + (59097/2)*n^3 -
 *            (3528381/40)*n^2 - (946929/20)*n + 153513 for n > 3 (returns 0
 *            for n = 1,2,3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 79, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    /* 128‑bit intermediate to avoid overflow for the range where the result
       fits in 64 bits, while still working with arbitrary‑precision seq_t. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;
    unsigned __int128 n5 = n4 * nn;
    unsigned __int128 n6 = n5 * nn;
    unsigned __int128 n7 = n6 * nn;
    unsigned __int128 n8 = n7 * nn;
    unsigned __int128 n9 = n8 * nn;
    unsigned __int128 n10 = n9 * nn;

    unsigned __int128 pos = (81u * n10) + (23490u * n7) +
                            (644295u * n4) + (1181940u * n3) + 6140520u;
    unsigned __int128 neg = (810u * n9) + (135u * n8) + (41760u * n6) +
                            (254142u * n5) + (3528381u * n2) + (1893858u * nn);
    unsigned __int128 diff = pos - neg;
    return (seq_t)(diff / 40u);
}
