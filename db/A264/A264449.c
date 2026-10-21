/*
 * A264449  a(n) = n*(n + 7)*(n + 14)*(n + 21)*(n + 28)/120.
 * https://oeis.org/A264449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 7)*(n + 14)*(n + 21)*(n + 28)/120
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid intermediate overflow for
       64‑bit builds; the result is cast back to seq_t.            */
    unsigned __int128 prod =
        (unsigned __int128)(n + 0) *
        (unsigned __int128)(n + 7) *
        (unsigned __int128)(n + 14) *
        (unsigned __int128)(n + 21) *
        (unsigned __int128)(n + 28);

    prod /= 120;                     /* exact division        */
    return (seq_t)prod;              /* cast to the desired type */
}
